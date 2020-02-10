/**
 * This class implements the TeamVisitor interface
 * and it basically computes the score points of 
 * any team by the competition rules.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public class TeamDisplayVisitor implements TeamVisitor {

	@Override
	public double visit(BasketballTeam basketballTeam) {

		double sum = 0;

		for (Player p : basketballTeam.getPlayers()) {
			sum += p.getScore();
		}

		return (double)(sum / basketballTeam.getNumberOfPlayers());
	}

	@Override
	public double visit(FootballTeam footballTeam) {
		String gender = footballTeam.getGender().substring(1);

		if (gender.equals("feminin")) {

			double min = Integer.MAX_VALUE;
			double sum = 0;

			for (Player p : footballTeam.getPlayers()) {
				if (p.getScore() < min) {
					min = p.getScore();
				}

				sum += p.getScore();
			}

			return (double)(min + sum);
		} else {

			double max = -1;
			double sum = 0;

			for (Player p : footballTeam.getPlayers()) {
				if (p.getScore() > max) {
					max = p.getScore();
				}

				sum += p.getScore();
			}

			return (double)(max + sum);
		}
	}

	@Override
	public double visit(HandballTeam handballTeam) {
		String gender = handballTeam.getGender().substring(1);

		if (gender.equals("feminin")) {

			double prod = 1;

			for (Player p : handballTeam.getPlayers()) {
				prod *= p.getScore();
			}

			return prod;
		} else {

			double sum = 0;

			for (Player p : handballTeam.getPlayers()) {
				sum += p.getScore();
			}

			return sum;
		}
	}
}
