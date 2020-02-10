/**
 * IS A "Entity" and HAS A "Person". It has details for 
 * an array of persons.
 * @author Ghiculescu Alexandru
 */
public class Group extends Entity {

	/* static variable for group priority points */
	private final static int GROUP_POINTS = 5;

	private Person members[]; /* group contains an array of persons*/
	private int cnt; 		  /* holds the number of group members*/

	/**
	 * Constructor with a given ID.
	 * @param id the id for the group
	 */
	public Group(String id) {
		super(id);
		members = new Person[100];
		this.cnt = 0;
	}

	/**
	 * Adds a member to the group.
	 * @param p the person to be added as a member of group
	 */
	public void addMember(Person p) {
		this.members[cnt] = p;
		cnt++;
	}

	/**
	 * Deletes a member from the group.
	 * @param name the name of the person to be deleted from the group
	 */
	public void deleteMember(String name) {
		for (int i = 0; i < cnt; i++) {

			if (members[i].getName().equals(name)) {

				for (int j = i; j < cnt - 1; j++) {
					members[j] = members[j + 1];
				}

				break;
			}
		}
		cnt--;
	}

	/**
	 * Sums the total priority of a group.
	 * @return the total priority of a group
	 */
	@Override
	public int getPriority() {
		int totalPriority = 0;

		for (int i = 0; i < cnt; i++) {
			totalPriority += members[i].getPriority();
		}

		return totalPriority + GROUP_POINTS;
	}

	/**
	 * Prints the group ID
	 */
	@Override
	public void PrintID() {
		System.out.println(super.getID());
	}

	/**
	 * Returns the number of members.
	 * @return the number of group members
	 */
	@Override
	public int getCnt() {
		return this.cnt;
	}
}
