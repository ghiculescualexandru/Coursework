import java.io.File;
import java.lang.String;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.FilterWriter;
import java.io.IOException;
import java.util.Scanner;
import java.util.HashMap;

/**
 * Contains main method and the read of 
 * passangers and actions.
 * @author Ghiculescu Alexandru
 */
public class ReadMain {

	/**
	 * Reads and store details of every person.
	 * 
	 * @param sc The scanner for reading persons
	 * @return A hash map containing all Entity to perform actions with
	 */
	public static HashMap<String, Entity> readPassangers(Scanner sc) {

		/* the hash map to store Entity with the ID being the key */
		HashMap<String, Entity> map = new HashMap<String, Entity>();

		/* the total number of persons to be read */
		int cntPassangers = sc.nextInt();

		while (cntPassangers != 0) {

			/* read and store all details for a person */
			String id = sc.next();
			String name = sc.next();
			int age = sc.nextInt();
			String ticketType = sc.next();
			boolean priorityBoarding = sc.nextBoolean();
			boolean needs = sc.nextBoolean();

			PriorityBoarding hasPriority = new PriorityBoarding(priorityBoarding);
			SpecialNeeds hasNeeds = new SpecialNeeds(needs);
			Ticket ticket = new Ticket(ticketType.charAt(0));

			/* construct the person with all the data read */
			Person person = new Person(name, age, ticket, hasPriority, hasNeeds);

			/*
			 * adding the person to an existing Entity if possible, otherwise create a new
			 * Entity and add the person, depending on what kind of Entity will be used
			 * (id[0]: s/f/g)
			 */
			switch (id.charAt(0)) {
			case 's':
				map.put(id, new Single(person, id));

				break;

			case 'g':
				if (map.containsKey(id) == false) {
					map.put(id, new Group(id));
				}
				((Group) map.get(id)).addMember(person);

				break;

			case 'f':
				if (map.containsKey(id) == false) {
					map.put(id, new Family(id));
				}
				((Family) map.get(id)).addMember(person);

				break;
			}
			cntPassangers--;
		}
		return map;
	}

	/**
	 * Reads and performs actions.
	 * 
	 * @param map The hashmap with all persons to perform actions with
	 * @param sc  The scanner for reading actions
	 * @throws IOException 
	 */
	public static void readActions(HashMap<String, Entity> map, Scanner sc, FileWriter fw) throws IOException {

		/*
		 * the priority queue with its heap and the dimension of the hashmap size (the
		 * number of persons)
		 */
		PriorityQueue priorityQueue = new PriorityQueue(map.size(), fw);

		while (sc.hasNext()) {
			/* reading an entire line and splitting it in a string array */
			String str = sc.nextLine();
			String action[] = str.split(" ");

			/*
			 * based on the first word of the action, call the priority queue methods
			 * desired
			 */
			switch (action[0]) {
			case "insert":
				priorityQueue.insert(map.get(action[1]), map.get(action[1]).getPriority());
				break;

			case "embark":
				priorityQueue.embark();
				break;

			case "list":
				priorityQueue.list();
				break;

			/*
			 * for delete, first check if we delete an entire entity or just a member by
			 * checking the length of the string array. If it has length equal to 2, it
			 * means we delete an entire Entity, otherwise we just delete a person from an
			 * Entity
			 */
			case "delete":
				if (action.length == 2) {
					priorityQueue.setToDelete(false);
				} else if (action.length == 3) {
					priorityQueue.setToDelete(true);
					priorityQueue.setPersonToDelete(action[2]);
				}

				priorityQueue.delete(map.get(action[1]));
			}
		}
	}

	/**
	 * Main method.
	 * 
	 * @param args
	 * @throws FileNotFoundException
	 */
	public static void main(String[] args) throws FileNotFoundException {

		File file = new File("./queue.in");
		Scanner sc = new Scanner(file);
		FileWriter fw;
		try {
			fw = new FileWriter("./queue.out");
			readActions(readPassangers(sc), sc, fw);
			fw.close();
		} catch (Exception e) {
			System.out.println(e);
		}	
	}
}
