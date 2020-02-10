/**
 * The team visitor interface.
 * Its methods returns the points of a team.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public interface TeamVisitor {

	public double visit(BasketballTeam basketballTeam);
	public double visit(FootballTeam footballTeam);
	public double visit(HandballTeam handballTeam);
}
