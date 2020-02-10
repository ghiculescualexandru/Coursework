/**
 * This class represents a temperature observation and its data consists of the
 * timestamp and the value of the temperature.
 * 
 * @author Ghiculescu Alexandru
 */
public class TemperatureObservation {

	/**
	 * Timestamp of the observation.
	 */
	private long timestamp;
	/**
	 * Value of temperature for the observation.
	 */
	private double temperature;

	/**
	 * Default constructor with no parameters.
	 */
	public TemperatureObservation() {
		this.timestamp = 0;
		this.temperature = 0;
	}

	/**
	 * Constructor with all given parameters.
	 * 
	 * @param time the timestamp of the observation
	 * @param temp the value for the temperature
	 */
	public TemperatureObservation(long time, double temp) {
		this.timestamp = time;
		this.temperature = temp;
	}

	/**
	 * Getter for the timestamp.
	 * 
	 * @return the timestamp of the observation
	 */
	public long getTimestamp() {
		return timestamp;
	}

	/**
	 * Setter for the timestamp.
	 * 
	 * @param timestamp the timestamp to be set
	 */
	public void setTimestamp(long timestamp) {
		this.timestamp = timestamp;
	}

	/**
	 * Getter for the temperature.
	 * 
	 * @return the temperature of the observation
	 */
	public double getTemperature() {
		return temperature;
	}

	/**
	 * Setter for the temperature.
	 * 
	 * @param temperature to be set
	 */
	public void setTemperature(double temperature) {
		this.temperature = temperature;
	}
}
