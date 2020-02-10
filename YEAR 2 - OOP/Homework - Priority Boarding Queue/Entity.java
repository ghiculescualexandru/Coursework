/**
 * The main class, abstract, parent for
 * Single, Group and Family.
 * @author Ghiculescu Alexandru
 */
public abstract class Entity {
	
	private String ID;		/* the unique ID of one entity */
	private int priority;	/* the priority for one entity */

	
	public Entity() {
	}
	
	/** 
	 * Constructor with a given ID.
	 * @param id the entity given ID
	 */
	public Entity(String id) {
		this.ID = id;
	}
	
	/**
	 * Returns the Entity's ID.
	 * @return the Entity's ID.
	 */
	public String getID() {
		return this.ID;
	}
	
	/**
	 * Sets a final priority for an Entity.
	 * @param priority is the final priority to be set 
	 * for an entity
	 */
	public void setFinalPriority(int priority) {
		this.priority = priority;
	}
	
	/**
	 * Returns the final priority of the Entity.
	 * @return the Entity's final priority.
	 */
	public int getFinalPriority() {
		return this.priority;
	}
	
	/* Abstract methods implemented in Single, Group and Family */
	/**
	 * Displays the Entity's ID.
	 */
	public abstract void PrintID();	
	/**
	 * Returns the Entity's priority.
	 * @return the priority of the Entity.
	 */
	public abstract int getPriority();
	/**
	 * Returns 1 for single and the number of memebers for
	 * Family and Group.
	 * @return the number of Entity members.
	 */
	public abstract int getCnt();
}
