import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

/**
 * This class represents the whole house, with its room and global data. The
 * global data consists of the temperature and humidity desired and the
 * timestamp reference.
 * 
 * @author Ghiculescu Alexandru
 */
public class House {

	/**
	 * The room array for the house.
	 */
	private ArrayList<Room> rooms;
	/**
	 * The house temperature desired.
	 */
	private double globalTemperature;
	/**
	 * The house humidity desired.
	 */
	private double globalHumidity;
	/**
	 * The reference timestamp.
	 */
	private long globalTimestamp;

	/**
	 * Default constructor with no parameters.
	 */
	public House() {
		this.rooms = new ArrayList<Room>();
		this.globalTemperature = 0;
		this.globalHumidity = 0;
		this.globalTimestamp = 0;
	}

	/**
	 * Constructor for the House class with all given parameters.
	 * 
	 * @param nr          the number of rooms for the house
	 * @param temperature the global temperature desired
	 * @param humidity    the global humidity desired
	 * @param timestamp   the timestamp reference
	 */
	public House(int nr, double temperature, double humidity, long timestamp) {
		this.rooms = new ArrayList<Room>(nr);
		this.globalTemperature = temperature;
		this.globalHumidity = humidity;
		this.globalTimestamp = timestamp;
	}

	/**
	 * Getter for global temperature.
	 * 
	 * @return the global temperature desired
	 */
	public double getGlobalTemperature() {
		return globalTemperature;
	}

	/**
	 * Setter for global temperature.
	 * 
	 * @param globalTemperature the global temperature desired to be set
	 */
	public void setGlobalTemperature(double globalTemperature) {
		this.globalTemperature = globalTemperature;
	}

	/**
	 * Getter for global humidity.
	 * 
	 * @return the global humidity desired
	 */
	public double getGlobalHumidity() {
		return globalHumidity;
	}

	/**
	 * Setter for global humidity
	 * 
	 * @param globalHumidity the global humidity desired to be set
	 */
	public void setGlobalHumidity(double globalHumidity) {
		this.globalHumidity = globalHumidity;
	}

	/**
	 * Returns the global temperature desired.
	 * 
	 * @param t the global temperature
	 */
	public void temperature(double t) {
		this.globalTemperature = t;
	}

	/**
	 * This method adds a room with its details into the array of rooms.
	 * 
	 * @param r the room to be added in the house
	 */
	public void addRoom(Room r) {
		this.rooms.add(r);
	}

	/**
	 * This method adds an observation to the hour needed.
	 * 
	 * @param hour        the hour where the observation will be added
	 * @param o           the observation to be added
	 * @param idToObserve the device id where the observation will be added
	 */
	public void addTemperatureObservation(int hour, TemperatureObservation o, String idToObserve) {
		for (Room r : this.rooms) {
			if (r.getDeviceId().equals(idToObserve)) {
				r.addTemperatureObservation(hour, o);
			}
		}
	}

	/**
	 * This method adds an observation to the hour needed.
	 * 
	 * @param hour        the hour where the observation will be added
	 * @param o           the observation to be added
	 * @param idToObserve the device id where the observation will be added
	 */
	public void addHumidityObservation(int hour, HumidityObservation o, String idToObserve) {
		for (Room r : this.rooms) {
			if (r.getDeviceId().equals(idToObserve)) {
				r.addHumidityObservation(hour, o);
			}
		}
	}

	/**
	 * This method computes the weighted average of the temperature and humidity (if
	 * a global humidity has been previously set). The weighted average depends on
	 * the area of the room. If a room does not have a temperature or humidity
	 * observation registered yet, the method does not take into consideration that
	 * room or the area of that room.
	 * 
	 * @return the answer for the trigger heat command
	 */
	public String triggerHeat() {

		/* Computing the weighted average of the temperature with the given formula. */
		double totalArea = 0;
		double sum = 0;
		for (Room r : this.rooms) {
			if (r.lastHourMinimumTemp() == -1) {
				continue;
			}

			totalArea += r.getArea();
			sum += (r.lastHourMinimumTemp() * r.getArea());
		}

		/*
		 * Computing the weighted average of the humidity with the given formula, only
		 * if there is a global humidity desired.
		 */
		double totalAreaHumid = 0;
		double sumHumid = 0;
		if (this.globalHumidity != 0) {
			for (Room r : this.rooms) {
				if (r.lastHourMaximumHumid() == -1) {
					continue;
				}
				totalAreaHumid += r.getArea();
				sumHumid += (r.lastHourMaximumHumid() * r.getArea());
			}
			/* Returning the answer. */
			if (sum / totalArea < this.globalTemperature && sumHumid / totalAreaHumid < this.globalHumidity) {
				return "YES";
			} else {
				return "NO";
			}
		}

		/* Returning the answer. */
		if (sum / totalArea < this.globalTemperature) {
			return "YES";
		} else {
			return "NO";
		}
	}

	/**
	 * This method writes in a file every list of temperature in ascending order,
	 * without duplicates for every complete time series in between the start
	 * timestamp and stop timestamp.
	 * 
	 * @param fw         the file writer
	 * @param roomNumber the number of rooms in the house
	 * @param startTime  the start timestamp
	 * @param stopTime   the stop timestamp
	 * @throws IOException file writer error
	 */
	public void list(FileWriter fw, int roomNumber, long startTime, long stopTime) throws IOException {
		rooms.get(roomNumber - 1).list(fw, startTime, stopTime, this.globalTimestamp);
	}

}
