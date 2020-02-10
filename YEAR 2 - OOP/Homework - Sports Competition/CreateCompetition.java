import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * This class uses Singleton and handles the reading of input and creating of a
 * competition.
 * 
 * It also handles the games between each team and displays the results.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public class CreateCompetition {

	/**
	 * Singleton for this class.
	 */
	private static CreateCompetition uniqueInstance;

	/**
	 * Creates the object if not created and returns it anyway.
	 * 
	 * @return the team created instance
	 */
	public static CreateCompetition getInstance() {
		if (uniqueInstance == null) {
			uniqueInstance = new CreateCompetition();
		}

		return uniqueInstance;
	}

	
	/**
	 * This method handles the read of the input and the creation of the competition.
	 * 
	 * @param teams the list of teams.
	 * @param input2 the input for the competition
	 * @return the competition created.
	 * @throws FileNotFoundException for file not found.
	 */
	public Competition createCompetition(ArrayList<Team> teams, String input2) throws FileNotFoundException {
		Scanner sc = new Scanner(new File(input2));

		ArrayList<Team> newTeams = new ArrayList<Team>();
		ArrayList<Team> newRanking = new ArrayList<Team>();

		String str = sc.nextLine();
		String arr[] = str.split("[, ]");

		String type = arr[0];
		String gender = arr[2];

		while (sc.hasNext()) {
			String teamName = sc.next();
			for (Team t : teams) {

				String teamNameToCompare = t.getTeamName().substring(1);
				String genderToCompare = t.getGender().substring(1);

				if (teamName.equals(teamNameToCompare) && genderToCompare.equals(gender)) {
					newTeams.add(t);
					newRanking.add(t);
				}
			}
		}

		Competition competition = new Competition(type, gender, newTeams, newRanking);

		sc.close();

		return competition;

	}

	/**
	 * This method basically handles the game between each team
	 * and displays the final results.
	 * 
	 * @param action the action to be done.
	 * @param input the input file.
	 * @param input2 the second input file.
	 * @param output the output file.
	 * @param allTeams the list with teams.
	 * @throws IOException for file not found.
	 */
	public void handler(String action, String input, String input2, String output, ArrayList<Team> allTeams)
			throws IOException {

		FileWriter fw = null;
		try {
			fw = new FileWriter(output);
		} catch (Exception e) {
			System.out.println(e);
		}

		Competition competition = createCompetition(allTeams, input2);
		CompetitionObserver competitionObserver = new CompetitionObserver(competition);

		for (int i = 0; i < competition.getTeams().size(); i++) {
			for (int j = i + 1; j < competition.getTeams().size(); j++) {

				if (competition.getTeams().get(i).accept(new TeamDisplayVisitor()) > competition.getTeams().get(j)
						.accept(new TeamDisplayVisitor())) {
					competition.getTeams().get(i).setScorePoints(competition.getTeams().get(i).getScorePoints() + 3);
				} else if (competition.getTeams().get(i).accept(new TeamDisplayVisitor()) < competition.getTeams()
						.get(j).accept(new TeamDisplayVisitor())) {
					competition.getTeams().get(j).setScorePoints(competition.getTeams().get(j).getScorePoints() + 3);
				} else {
					competition.getTeams().get(i).setScorePoints(competition.getTeams().get(i).getScorePoints() + 1);
					competition.getTeams().get(j).setScorePoints(competition.getTeams().get(j).getScorePoints() + 1);
				}
				
				competition.notifyAllObservers();
			}
		}

		for (int i = 0; i < 3; i++) {
			fw.append(competition.getRanking().get(i).getTeamName().substring(1));
			fw.append("\n");
		}

		for (int i = 0; i < competition.getTeams().size(); i++) {
			fw.append("Echipa" + competition.getTeams().get(i).getTeamName() + " a ocupat locul "
					+ competition.getTeams().get(i).getTeamPlace());
			fw.append("\n");
		}

		fw.close();
	}
}
