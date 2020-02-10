/**
 * This class is a basic class for a player of a team.
 * It has basic methods and overrides toString to display
 * all the details of the player in the right format.
 * @author Ghiculescu Alexandru
 *
 */
public class Player {

	/**
	 * The name of the player.
	 */
	private String name;
	/**
	 * The score of the player.
	 */
	private int score;

	/**
	 * Default constructor. 
	 */
	public Player() {
		this.name = "";
		this.score = 0;
	}

	/**
	 * Constructor with all given parameters.
	 * @param name the name of the player.
	 * @param score the score of the player.
	 */
	public Player(String name, int score) {
		this.name = name;
		this.score = score;
	}

	/**
	 * Getter for the name.
	 * @return the name of the player.
	 */
	public String getName() {
		return name;
	}

	/**
	 * Setter for the name.
	 * @param name the name to be set.
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * Getter for the score.
	 * @return the score of the player.
	 */
	public int getScore() {
		return score;
	}

	/**
	 * Setter for the score.
	 * @param score the score to be set.
	 */
	public void setScore(int score) {
		this.score = score;
	}

	/**
	 * This overrides the method toString in order 
	 * to display corectely the details for the player.
	 */
	@Override
	public String toString() {
		return "{name: " + this.name + ", score: " + this.score + "}";
	}
}
