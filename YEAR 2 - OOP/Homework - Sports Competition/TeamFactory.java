/**
 * This class uses Factory to create a new team.
 * @author Ghiculescu Alexandru
 *
 */
public class TeamFactory {

	/**
	 * Singleton is used because only an instance
	 * of TeamFactory is wanted.
	 */
	private static TeamFactory uniqueInstance;

	/**
	 * Creates the object if not created and
	 * returns it anyway.
	 * @return the teamFactory instance
	 */
	public static TeamFactory getInstance() {
		if (uniqueInstance == null) {
			uniqueInstance = new TeamFactory();
		}

		return uniqueInstance;
	}

	/**
	 * This method creates and returns a new team with
	 * all the specific attributes.
	 * @param type the type of the team.
	 * @param teamName the name of the team.
	 * @param gender the gender of the players.
	 * @param numberOfPlayers the number of players in the team.
	 * @return the new team created.
	 */
	public Team createTeam(String type, String teamName, String gender, int numberOfPlayers) {
		Team team = null;
		
		if(type.equals("football")) {
			team = new FootballTeam(teamName, gender, numberOfPlayers);
		} else if (type.equals("basketball")) {
			team = new BasketballTeam(teamName, gender, numberOfPlayers);
		} else if (type.equals("handball")) {
			team = new HandballTeam(teamName, gender, numberOfPlayers);
		}
		
		return team;
	}
}
