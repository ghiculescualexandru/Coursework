/**
 * IS A "Entity" and HAS A "Person". It has details for 
 * an array of persons.
 * @author Ghiculescu Alexandru
 */
public class Family extends Entity {

	/* static variable for family priority points */
	private final static int FAMILY_POINTS = 10;

	private Person members[]; /* family contains an array of persons*/
	private int cnt; 		  /* holds the number of family members*/

	/**
	 * Constructor with a given ID.
	 * @param id the id for the family
	 */
	public Family(String id) {
		super(id);
		members = new Person[100];
		this.cnt = 0;
	}

	/**
	 * Adds a member to the family.
	 * @param p the person to be added as a member of family
	 */
	public void addMember(Person p) {
		this.members[cnt] = p;
		cnt++;
	}

	/**
	 * Deletes a member from the family.
	 * @param name the name of the person to be deleted from the family
	 */
	public void deleteMember(String name) {
		for (int i = 0; i < cnt; i++) {

			if (members[i].getName().equals(name)) {

				for (int j = i; j < cnt - 1; j++) {
					members[j] = members[j + 1];
				}
			}
		}
		cnt--;
	}

	/**
	 * Sums the total priority of the family.
	 * @return the total priority of a family
	 */
	@Override
	public int getPriority() {
		int totalPriority = 0;

		for (int i = 0; i < cnt; i++) {
			totalPriority += members[i].getPriority();
		}

		return totalPriority + FAMILY_POINTS;
	}

	/**
	 * Prints the family ID
	 */
	@Override
	public void PrintID() {
		System.out.println(super.getID());
	}

	/**
	 * Returns the number of members.
	 * @return the number of family members
	 */
	@Override
	public int getCnt() {
		return this.cnt;
	}
}
