import java.util.ArrayList;

/**
 * This class is a team with its main attributes: name and players.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public abstract class Team {

	/**
	 * The name of the team.
	 */
	private String teamName;
	/**
	 * The gender of the players.
	 */
	private String gender;
	/**
	 * The number of players in the team.
	 */
	private int numberOfPlayers;
	/**
	 * The list of players in the team.
	 */
	private ArrayList<Player> players = new ArrayList<Player>();

	/**
	 * The score points of the team.
	 */
	private double scorePoints;
	/**
	 * The place in the ordered list.
	 */
	private int teamPlace;

	/**
	 * Default constructor.
	 */
	public Team() {
		this.teamName = "";
		this.gender = "";
		this.numberOfPlayers = 0;
		this.scorePoints = 0;
	}

	/**
	 * Constructor with all given parameters.
	 * 
	 * @param teamName        the team name.
	 * @param gender          the gender of players.
	 * @param numberOfPlayers the number of players.
	 */
	public Team(String teamName, String gender, int numberOfPlayers) {
		this.teamName = teamName;
		this.gender = gender;
		this.numberOfPlayers = numberOfPlayers;
	}

	/**
	 * Getter for name.
	 * 
	 * @return the team name.
	 */
	public String getTeamName() {
		return teamName;
	}

	/**
	 * Setter for name.
	 * 
	 * @param teamName the name to be set.
	 */
	public void setTeamName(String teamName) {
		this.teamName = teamName;
	}

	/**
	 * Getter for gender.
	 * 
	 * @return the gender.
	 */
	public String getGender() {
		return gender;
	}

	/**
	 * Setter for gender
	 * 
	 * @param gender the gender to be set.
	 */
	public void setGender(String gender) {
		this.gender = gender;
	}

	/**
	 * Getter for the number of players.
	 * 
	 * @return the number of players.
	 */
	public int getNumberOfPlayers() {
		return numberOfPlayers;
	}

	/**
	 * Setter for the number of players.
	 * 
	 * @param numberOfPlayers the nr to be set.
	 */
	public void setNumberOfPlayers(int numberOfPlayers) {
		this.numberOfPlayers = numberOfPlayers;
	}

	/**
	 * Getter for the list of players.
	 * 
	 * @return the list of players.
	 */
	public ArrayList<Player> getPlayers() {
		return players;
	}

	/**
	 * Setter for the list of players.
	 * 
	 * @param players the list to be set.
	 */
	public void setPlayers(ArrayList<Player> players) {
		this.players = players;
	}

	/**
	 * This method adds a player into the list of players.
	 * 
	 * @param player the player to be added into the list.
	 */
	public void addPlayer(Player player) {
		this.players.add(player);
	}

	/**
	 * This method display the details of the team in the right manner.
	 * 
	 * @return the string to display.
	 */
	public String displayTeam() {
		return "teamName:" + this.getTeamName() + ", gender:" + this.getGender() + ", numberOfPlayers: "
				+ this.getNumberOfPlayers() + ", ";
	}

	/**
	 * Getter for the score points.
	 * @return the score points.
	 */
	public double getScorePoints() {
		return scorePoints;
	}

	/**
	 * Setter for the score points.
	 * @param scorePoints points to be set.
	 */
	public void setScorePoints(double scorePoints) {
		this.scorePoints = scorePoints;
	}

	/**
	 * Getter for the team place.
	 * @return the team place.
	 */
	public int getTeamPlace() {
		return teamPlace;
	}

	/**
	 * Setter for the team place.
	 * @param teamPlace the place to be set.
	 */
	public void setTeamPlace(int teamPlace) {
		this.teamPlace = teamPlace;
	}

	/**
	 * The accept method for the competition.
	 * @param teamVisitor a visitor for the team.
	 * @return the team points.
	 */
	public abstract double accept(TeamVisitor teamVisitor);
}
