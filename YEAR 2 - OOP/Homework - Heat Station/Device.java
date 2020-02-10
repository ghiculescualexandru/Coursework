import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Set;
import java.util.TreeSet;

/**
 * This class represents the device from a room and hold both temperature and
 * humidity observations for the last 24 hours from the timestamp reference,
 * with its methods. Basically, for both humidity and temperature, it has 24
 * lists, every element being a treeset with observations. The index 0 of the
 * list is the most recent hour, last 3600 seconds, the index 1 of the list is
 * the second most recent hour, 7200 seconds - 3601 seconds ago and so on.
 * 
 * @author Ghiculescu Alexandru
 */
public class Device {
	/**
	 * Static variable representing the number of seconds in an hour.
	 */
	private static final int ONE_HOUR = 3600;
	/**
	 * The room number of the device.
	 */
	private String roomNumber;
	/**
	 * The device id.
	 */
	private String deviceId;
	/**
	 * Array list of array list for temperature observations.
	 */
	private ArrayList<TreeSet<TemperatureObservation>> lastHours;
	/**
	 * Array list of array list for humiditity observations.
	 */
	private ArrayList<TreeSet<HumidityObservation>> lastHoursHumidity;

	/**
	 * Default constructors with no parameters.
	 */
	public Device() {
		this.roomNumber = "";
		this.deviceId = "";
		this.lastHours = new ArrayList<TreeSet<TemperatureObservation>>();
		this.lastHoursHumidity = new ArrayList<TreeSet<HumidityObservation>>();
	}

	/**
	 * Constructor for the device with all given parameters.
	 * 
	 * @param id         the device id
	 * @param roomNumber the room number of the device id
	 */
	public Device(String id, String roomNumber) {
		this.roomNumber = roomNumber;
		this.deviceId = id;
		this.lastHours = new ArrayList<TreeSet<TemperatureObservation>>();
		this.lastHoursHumidity = new ArrayList<TreeSet<HumidityObservation>>();
		/* Constructing 24 lists for both temperature and humidity observations. */
		for (int i = 0; i < 24; i++) {
			Set<TemperatureObservation> temperatureSet = new TreeSet<>(new Comparator<TemperatureObservation>() {
				public int compare(TemperatureObservation o1, TemperatureObservation o2) {
					return (int) Math.signum(o1.getTemperature() - o2.getTemperature());
				}
			});
			Set<HumidityObservation> humiditySet = new TreeSet<>(new Comparator<HumidityObservation>() {
				public int compare(HumidityObservation o1, HumidityObservation o2) {
					return (int) Math.signum(o1.getHumidity() - o2.getHumidity());
				}
			});
			this.lastHours.add(i, (TreeSet<TemperatureObservation>) temperatureSet);
			this.lastHoursHumidity.add(i, (TreeSet<HumidityObservation>) humiditySet);
		}
	}

	/**
	 * Getter for the room number.
	 * 
	 * @return the room number
	 */
	public String getRoomNumber() {
		return roomNumber;
	}

	/**
	 * Setter for the room number.
	 * 
	 * @param roomNumber the room number to be set
	 */
	public void setRoomNumber(String roomNumber) {
		this.roomNumber = roomNumber;
	}

	/**
	 * Getter for the device id.
	 * 
	 * @return the device id
	 */
	public String getDeviceId() {
		return deviceId;
	}

	/**
	 * Setter for the device id.
	 * 
	 * @param deviceId the device id to be set
	 */
	public void setDeviceId(String deviceId) {
		this.deviceId = deviceId;
	}

	/**
	 * This method adds an observation in the sublist for the hour needed.
	 * 
	 * @param hour                   the hour where the temperature observation will
	 *                               be added
	 * @param temperatureObservation the temperature observation to be added
	 */
	public void addTemperatureObservation(int hour, TemperatureObservation temperatureObservation) {
		this.lastHours.get(hour).add(temperatureObservation);
	}

	/**
	 * This method adds an observation in the sublist for the hour needed.
	 * 
	 * @param hour                the hour where the humidity observation will be
	 *                            added
	 * @param humidityObservation the humidity observation to be added
	 */
	public void addHumidityObservation(int hour, HumidityObservation humidityObservation) {
		this.lastHoursHumidity.get(hour).add(humidityObservation);
	}

	/**
	 * This method writes in a file every list of temperature in ascending order,
	 * without duplicates for every complete time series in between the start
	 * timestamp and stop timestamp.
	 * 
	 * @param fw              the file writer
	 * @param startTime       the start timestamp
	 * @param stopTime        the stop timestamp
	 * @param globalTimestamp the reference timestamp
	 * @throws IOException file writer error
	 */
	public void list(FileWriter fw, long startTime, long stopTime, long globalTimestamp) throws IOException {
		/*
		 * The difference between the reference timestamp and the start time and stop
		 * time.
		 */
		long startDiff = globalTimestamp - startTime;
		long stopDiff = globalTimestamp - stopTime;
		/*
		 * Computing the start hour and stop hour between the temperature values will be
		 * listed.
		 */
		int startHour = (int) (startDiff / ONE_HOUR);
		int stopHour = (int) (stopDiff / ONE_HOUR);

		fw.append(this.roomNumber + " ");
		StringBuilder builder = new StringBuilder();
		/*
		 * Displaying the values for the temperature between the two given timestamps.
		 */
		for (int i = stopHour; i <= startHour - 1; i++) {
			Set<TemperatureObservation> subList = this.lastHours.get(i);

			for (TemperatureObservation o : subList) {
				String str = String.format("%,.2f", o.getTemperature()).replace(",", ".");
				builder.append(str + " ");
			}
		}

		builder.setLength(builder.length() - 1);
		fw.append(builder + "\n");
	}

	/**
	 * Returns the minimum temperature in the last hour observed by searching for
	 * the most recent hour which has at least one element.
	 * 
	 * @return the minimum temperature in the last hour observed
	 */
	public double lastHourMinimumTemp() {
		TreeSet<TemperatureObservation> subList;

		/* Finding the most recent hour with given observations. */
		for (int i = 0; i < 24; i++) {
			subList = this.lastHours.get(i);
			if (!subList.isEmpty()) {
				/* Returning the minimum value of temperature in the last hour observed. */
				return subList.first().getTemperature();
			}
		}
		return -1;
	}

	/**
	 * Returns the maximum humidity in the last hour observed by searching for the
	 * most recent hour which has at least one element.
	 * 
	 * @return the maximum humidity in the last hour observed
	 */
	public double lastHourMaximumHumid() {
		TreeSet<HumidityObservation> subList;

		/* Finding the most recent hour with given humidity observations. */
		for (int i = 0; i < 24; i++) {
			subList = this.lastHoursHumidity.get(i);
			if (!subList.isEmpty()) {
				/* Returning the maximum value of humidity in the last hour observed. */
				return subList.last().getHumidity();
			}
		}
		return -1;
	}
}
