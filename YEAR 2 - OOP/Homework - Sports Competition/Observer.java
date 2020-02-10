
/**
 * Default observer.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public abstract class Observer {

	/**
	 * The main attribute for the observer.
	 */
	protected Competition competition;
	
	/**
	 * Default constructor.
	 */
	public Observer() {
		
	}
	
	/**
	 * Constructor with all given parameters.
	 * @param c the competition.
	 */
	public Observer(Competition c) {
		this.competition = c;
		this.competition.attach(this);
	}

	/**
	 * Abstract update method.
	 */
	public abstract void update();
}
