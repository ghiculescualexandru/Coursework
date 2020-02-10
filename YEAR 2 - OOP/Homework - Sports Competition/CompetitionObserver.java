import java.util.Collections;
import java.util.Comparator;
/**
 * Observer for the competition.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public class CompetitionObserver extends Observer {

	/**
	 * Constructor for the observer.
	 * @param competition the competition.
	 */
	public CompetitionObserver(Competition competition) {
		super(competition);
	}

	/**
	 * This method sorts the team in descending order with a new
	 * comparator based only on score points.
	 * It also updates the ordered list of teams.
	 */
	@Override
	public void update() {
		Collections.sort(this.competition.getRanking(), new Comparator<Team>() {
			@Override
			public int compare(Team t1, Team t2) {
				return (int) Math.signum(t2.getScorePoints() - t1.getScorePoints());
			}
		});
		
		for(int i = 0; i < this.competition.getRanking().size(); i++) {
			this.competition.getRanking().get(i).setTeamPlace(i + 1);
		}
	}

}
