/**
 * Contains all details of one person.
 * @author Ghiculescu Alexandru
 */
public class Person {

	/* static variables for boarding and special needs priority */
	private final static int BOARDING_POINTS = 30;
	private final static int NEEDS_POINTS = 100;

	private int age; 							/* age */
	private String name;						/* name */
	private Ticket ticket;						/* ticket - returns priority based on type*/
	private PriorityBoarding priorityBoarding;	/* priority boarding */
	private SpecialNeeds specialNeeds;			/* special needs */
	private int priority;

	public Person() {
	}

	/**
	 * Constructor with all the data needed.
	 * @param name string for the passanger's name
	 * @param age integer for the age
	 * @param ticket returns priority based on type
	 * @param priorityBoarding returns if a passanger has priority boarding or not
	 * @param specialNeeds returns if a passanger has special needs or not
	 */
	public Person(String name, int age, Ticket ticket, PriorityBoarding priorityBoarding, SpecialNeeds specialNeeds) {
		this.age = age;
		this.name = name;
		this.ticket = ticket;
		this.priorityBoarding = priorityBoarding;
		this.specialNeeds = specialNeeds;
	}

	/**
	 * Returns the total priority.
	 * @return the total priority of a passanger based on ticket type,
	 * age, priority boarding and special needs.
	 */
	public int getPriority() {
		int currPriority = 0;

		if (this.priorityBoarding.getPriorityBoarding() == true) {
			currPriority += BOARDING_POINTS;
		}

		if (this.specialNeeds.getSpecialNeeds() == true) {
			currPriority += NEEDS_POINTS;
		}

		if (this.age < 2) {
			currPriority += 20;
		}

		if (this.age < 5 && this.age >= 2) {
			currPriority += 10;
		}

		if (this.age < 10 && this.age >= 5) {
			currPriority += 5;
		}

		if (this.age < 60 && this.age >= 10) {
			currPriority += 0;
		}

		if (this.age >= 60) {
			currPriority += 15;
		}

		currPriority += this.ticket.getPriority();

		return currPriority;
	}

	/**
	 * Sets the person's priority.
	 */
	public void setPriority(int priority) {
		this.priority = priority;
	}

	/**
	 * Returns the person's name.
	 * @return the person's name.
	 */
	public String getName() {
		return this.name;
	}
}
