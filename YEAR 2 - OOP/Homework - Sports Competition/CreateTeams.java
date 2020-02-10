import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * This class uses Singleton and handles the reading of input and creating of
 * teams.
 * 
 * It also displays all details about every added team.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public class CreateTeams {

	/**
	 * Singleton for this class.
	 */
	private static CreateTeams uniqueInstance;

	/**
	 * Creates the object if not created and returns it anyway.
	 * 
	 * @return the team created instance
	 */
	public static CreateTeams getInstance() {
		if (uniqueInstance == null) {
			uniqueInstance = new CreateTeams();
		}

		return uniqueInstance;
	}

	/**
	 * This method basically handles the input read, the teams creation and the
	 * display of details for every added team.
	 * 
	 * @param action the action to be done.
	 * @param input  the input file.
	 * @param unused unused.
	 * @param output the output file.
	 * @throws IOException for file not found.
	 * @return the list of teams.
	 */
	public ArrayList<Team> handler(String action, String input, String unused, String output) throws IOException {

		Scanner sc = new Scanner(new File(input));

		FileWriter fw = null;
		try {
			fw = new FileWriter(output);
		} catch (Exception e) {
			System.out.println(e);
		}

		ArrayList<Team> allTeams = new ArrayList<Team>();

		while (sc.hasNext()) {
			String buffer = sc.nextLine();
			String arr[] = buffer.split(",");

			if (arr[0].equals("football")) {
				arr[3] = arr[3].substring(1, arr[3].length());
				allTeams.add(
						TeamFactory.getInstance().createTeam("football", arr[1], arr[2], Integer.parseInt(arr[3])));

			} else if (arr[0].equals("basketball")) {
				arr[3] = arr[3].substring(1, arr[3].length());
				allTeams.add(
						TeamFactory.getInstance().createTeam("basketball", arr[1], arr[2], Integer.parseInt(arr[3])));

			} else if (arr[0].equals("handball")) {
				arr[3] = arr[3].substring(1, arr[3].length());
				allTeams.add(
						TeamFactory.getInstance().createTeam("handball", arr[1], arr[2], Integer.parseInt(arr[3])));

			} else {
				arr[1] = arr[1].substring(1, arr[1].length());
				allTeams.get(allTeams.size() - 1).addPlayer(new Player(arr[0], Integer.parseInt(arr[1])));
			}
		}

		for (Team team : allTeams) {
			fw.append("{");

			fw.append(team.displayTeam());

			fw.append("players: [");
			for (int i = 0; i < team.getPlayers().size(); i++) {
				fw.append(team.getPlayers().get(i).toString());

				if (i < team.getPlayers().size() - 1) {
					fw.append(", ");
				}
			}
			fw.append("]");

			fw.append("}");
			fw.append("\n");
		}

		sc.close();
		fw.close();

		return allTeams;
	}

	public ArrayList<Team> createOnly(String action, String input, String unused, String output) throws IOException {

		Scanner sc = new Scanner(new File(input));

		FileWriter fw = null;
		try {
			fw = new FileWriter(output);
		} catch (Exception e) {
			System.out.println(e);
		}

		ArrayList<Team> allTeams = new ArrayList<Team>();

		while (sc.hasNext()) {
			String buffer = sc.nextLine();
			String arr[] = buffer.split(",");

			if (arr[0].equals("football")) {
				arr[3] = arr[3].substring(1, arr[3].length());
				allTeams.add(
						TeamFactory.getInstance().createTeam("football", arr[1], arr[2], Integer.parseInt(arr[3])));

			} else if (arr[0].equals("basketball")) {
				arr[3] = arr[3].substring(1, arr[3].length());
				allTeams.add(
						TeamFactory.getInstance().createTeam("basketball", arr[1], arr[2], Integer.parseInt(arr[3])));

			} else if (arr[0].equals("handball")) {
				arr[3] = arr[3].substring(1, arr[3].length());
				allTeams.add(
						TeamFactory.getInstance().createTeam("handball", arr[1], arr[2], Integer.parseInt(arr[3])));

			} else {
				arr[1] = arr[1].substring(1, arr[1].length());
				allTeams.get(allTeams.size() - 1).addPlayer(new Player(arr[0], Integer.parseInt(arr[1])));
			}
		}
		
		sc.close();
		fw.close();
		
		return allTeams;
	}
}
