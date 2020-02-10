/**
 * This class represents a humidity observation and its data consists of the
 * timestamp and the value of the humidity.
 * 
 * @author Ghiculescu Alexandru
 */
public class HumidityObservation {

	/**
	 * Timestamp of the observation.
	 */
	private long timestamp;
	/**
	 * Value of humidity for the observation.
	 */
	private double humidity;

	/**
	 * Default constructor with no parameters.
	 */
	public HumidityObservation() {
		this.timestamp = 0;
		this.humidity = 0;
	}

	/**
	 * Constructor with all given parameters.
	 * 
	 * @param time  the timestamp of the observation
	 * @param humid the value for the humidity
	 */
	public HumidityObservation(long time, double humid) {
		this.timestamp = time;
		this.humidity = humid;
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
	 * Getter for the humidity.
	 * 
	 * @return the humidity of the observation
	 */
	public double getHumidity() {
		return humidity;
	}

	/**
	 * Setter for the humidity.
	 * 
	 * @param humidity to be set
	 */
	public void setHumidity(double humidity) {
		this.humidity = humidity;
	}
}
