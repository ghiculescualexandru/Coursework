/**
 * This class extends Team.
 * @author Ghiculescu Alexandru
 *
 */
public class HandballTeam extends Team {

	/**
	 * Default constructor.
	 */
	public HandballTeam() {

	}
	/**
	 * Constructor with all given parameters.
	 * @param teamName the name of the team.
	 * @param gender the gender of players.
	 * @param numberOfPlayers the number of players.
	 */
	public HandballTeam(String teamName, String gender, int numberOfPlayers) {
		super(teamName, gender, numberOfPlayers);
	}
	
	@Override
	public double accept(TeamVisitor teamVisitor) {
		return teamVisitor.visit(this);
	}
}
