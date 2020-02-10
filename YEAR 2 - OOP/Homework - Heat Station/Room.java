import java.io.FileWriter;
import java.io.IOException;

/**
 * This class represents one room of the house with its device and data. The
 * data consists of the device, its id and the room name and area.
 * 
 * @author Ghiculescu Alexandru
 */
public class Room {
	/**
	 * The room device.
	 */
	private Device device;
	/**
	 * The room area.
	 */
	private double area;
	/**
	 * The room name.
	 */
	private String name;
	/**
	 * The device ID of the room.
	 */
	private String deviceId;

	/**
	 * Default constructor with no parameters.
	 */
	public Room() {
		this.device = new Device();
		this.name = "";
		this.deviceId = "";
		this.area = 0;
	}

	/**
	 * Constructor with all given parameters.
	 * 
	 * @param name     the name of the room
	 * @param deviceId the device id
	 * @param area     the area of the room
	 */
	public Room(String name, String deviceId, double area) {
		this.name = name;
		this.deviceId = deviceId;
		this.device = new Device(deviceId, this.name);
		this.area = area;
	}

	/**
	 * Getter for the area.
	 * 
	 * @return the area of the room
	 */
	public double getArea() {
		return area;
	}

	/**
	 * Setter for the area.
	 * 
	 * @param area the area to be set
	 */
	public void setArea(double area) {
		this.area = area;
	}

	/**
	 * Getter for the name of the room.
	 * 
	 * @return the name of the room
	 */
	public String getName() {
		return name;
	}

	/**
	 * Setter for the name of the room.
	 * 
	 * @param name the name to be set
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * Getter for the device id.
	 * 
	 * @return the deviceId for the room
	 */
	public String getDeviceId() {
		return deviceId;
	}

	/**
	 * Setter for the device id.
	 * 
	 * @param deviceId to be set
	 */
	public void setDeviceId(String deviceId) {
		this.deviceId = deviceId;
	}

	/**
	 * This method adds a temperature observation in the sublist for the hour
	 * needed.
	 * 
	 * @param hour                   where the temperature observation will be added
	 * @param temperatureObservation the temperature observation to be added
	 */
	public void addTemperatureObservation(int hour, TemperatureObservation temperatureObservation) {
		this.device.addTemperatureObservation(hour, temperatureObservation);
	}

	/**
	 * This method adds an observation in the sublist for the hour needed.
	 * 
	 * @param hour                the hour where the humidity observation will be
	 *                            added
	 * @param humidityObservation the humidity observation to be added
	 */
	public void addHumidityObservation(int hour, HumidityObservation humidityObservation) {
		this.device.addHumidityObservation(hour, humidityObservation);
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
	 * @throws IOException file writing error
	 */
	public void list(FileWriter fw, long startTime, long stopTime, long globalTimestamp) throws IOException {
		this.device.list(fw, startTime, stopTime, globalTimestamp);
	}

	/**
	 * Returns the minimum temperature in the last hour observed by searching for
	 * the most recent hour which has at least one element.
	 * 
	 * @return the minimum temperature in the last hour observed
	 */
	public double lastHourMinimumTemp() {
		return this.device.lastHourMinimumTemp();
	}

	/**
	 * Returns the maximum humidity in the last hour observed by searching for the
	 * most recent hour which has at least one element.
	 * 
	 * @return the maximum humidity in the last hour observed
	 */
	public double lastHourMaximumHumid() {
		return this.device.lastHourMaximumHumid();
	}
}
