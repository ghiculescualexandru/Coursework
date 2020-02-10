/**
 * This class extends Team.
 * @author Ghiculescu Alexandru
 *
 */
public class FootballTeam extends Team {

	/**
	 * Default constructor.
	 */
	public FootballTeam() {
		
	}
	/**
	 * Constructor with all given parameters.
	 * @param teamName the name of the team.
	 * @param gender the gender of players.
	 * @param numberOfPlayers the number of players.
	 */
	public FootballTeam(String teamName, String gender, int numberOfPlayers) {
		super(teamName, gender, numberOfPlayers);
	}
	
	@Override
	public double accept(TeamVisitor teamVisitor) {
		return teamVisitor.visit(this);
	}
}
