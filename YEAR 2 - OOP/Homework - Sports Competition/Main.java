import java.io.IOException;
import java.util.ArrayList;

/**
 * Main class.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public class Main {
	/**
	 * Main method.
	 * 
	 * @param args the arguments for input and output file.
	 * @throws IOException for file not found.
	 */
	public static void main(String[] args) throws IOException {

		ArrayList<Team> allTeams;
		
		if (args[0].equals("inscriere")) {
			allTeams = CreateTeams.getInstance().handler(args[0], args[1], args[2], args[3]);			
		}
		
		
		if (args[0].equals("competitie")) {
			allTeams = CreateTeams.getInstance().createOnly(args[0], args[1], args[2], args[3]);
			CreateCompetition.getInstance().handler(args[0], args[1], args[2], args[3], allTeams);			
		}
	}

}
