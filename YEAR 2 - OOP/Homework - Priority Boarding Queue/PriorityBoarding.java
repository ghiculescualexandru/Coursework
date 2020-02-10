/**
 * Contains priority boarding as true or false.
 * @author Ghiculescu Alexandru
 */
public class PriorityBoarding {

	private boolean priorityBoarding; /* holds a boolean for priority boarding*/

	public PriorityBoarding() {
	}

	/**
	 * Constructor with a given boolean priority.
	 * @param p the boolean to be set for priorityBoarding
	 */
	public PriorityBoarding(boolean p) {
		this.priorityBoarding = p;
	}

	/**
	 * Returns the priority boarding as false or true.
	 * @return priority boarding as false or true.
	 */
	public boolean getPriorityBoarding() {
		return this.priorityBoarding;
	}

	
	/**
	 * Sets the priority boarding to false or true.
	 * @param p true or false for priority boarding.
	 */
	public void setPriorityBoarding(boolean p) {
		this.priorityBoarding = p;
	}
}
