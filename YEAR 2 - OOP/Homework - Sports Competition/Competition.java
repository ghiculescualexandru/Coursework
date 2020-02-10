import java.util.ArrayList;
import java.util.List;
/**
 * This class is a competition with all its main attributes,
 * such as: type, gender, all teams, observer etc.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public class Competition {

	/**
	 * The type of the competition.
	 */
	private String type;
	/**
	 * The gender of the players.
	 */
	private String gender;
	/**
	 * A list with the all the teams.
	 */
	private ArrayList<Team> teams;
	/**
	 * A list to be ordered for the competition.
	 */
	private ArrayList<Team> ranking;
	/**
	 * List of observers.
	 */
	private List<Observer> observers = new ArrayList<Observer>();

	/**
	 * Constructor with all given parameters.
	 * 
	 * @param type the type of the competition.
	 * @param gender the gender of the players.
	 * @param teams the list with all teams.
	 * @param ranking the list with all teams for ranking.
	 */
	public Competition(String type, String gender, ArrayList<Team> teams, ArrayList<Team> ranking) {
		super();
		this.type = type;
		this.gender = gender;
		this.teams = teams;
		this.ranking = ranking;
	}

	/**
	 * Returns the type of the competition.
	 * @return the type of the competition.
	 */
	public String getType() {
		return type;
	}

	/**
	 * Set the type of the competition.
	 * @param type the type to be set.
	 */
	public void setType(String type) {
		this.type = type;
	}

	/**
	 * Returns the gender of the players.
	 * @return the gender of the players.
	 */
	public String getGender() {
		return gender;
	}

	/**
	 * Setter for the gender.
	 * @param gender to be set.
	 */
	public void setGender(String gender) {
		this.gender = gender;
	}

	/**
	 * Returns the list withh al teams.
	 * @return the list with all teams.
	 */
	public ArrayList<Team> getTeams() {
		return teams;
	}

	/**
	 * Setter for the list of teams.
	 * @param teams the list to be set.
	 */
	public void setTeams(ArrayList<Team> teams) {
		this.teams = teams;
	}

	/**
	 * Attach for the observer.
	 * @param observer to be attached.
	 */
	public void attach(Observer observer) {
		observers.add(observer);
	}

	/**
	 * This method updates all observers.
	 */
	public void notifyAllObservers() {
		for (Observer observer : observers) {
			observer.update();
		}
	}

	/**
	 * Returns the ordered list of teams.
	 * @return the ordered list.
	 */
	public ArrayList<Team> getRanking() {
		return ranking;
	}

	/**
	 * Setter for the ordered list of teams.
	 * @param ranking the list to be set.
	 */
	public void setRanking(ArrayList<Team> ranking) {
		this.ranking = ranking;
	}

	/**
	 * Getter for the observers list.
	 * @return the observers list.
	 */
	public List<Observer> getObservers() {
		return observers;
	}

	/**
	 * Setter for the observers list.
	 * @param observers observers to be set.
	 */
	public void setObservers(List<Observer> observers) {
		this.observers = observers;
	}

}
