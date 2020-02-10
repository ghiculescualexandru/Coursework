import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
/**
 * HAS A Max Heap and implements the main 
 * actions read in main.
 * @author Ghiculescu Alexandru
 */
public class PriorityQueue {

	private MaxHeap heap;	/* priority queue has a max heap */
	private FileWriter fw;
	private boolean firstList = true;
	private boolean toDelete;
	private String personToDelete = new String();
	
	public PriorityQueue() {
	}

	/**
	 * Creates a heap with a given size.
	 * @param cnt The size of the heap.
	 */
	public PriorityQueue(int cnt, FileWriter fw) {
		this.heap = new MaxHeap(cnt);
		this.fw = fw;
	}

	/**
	 * Setter for the person to be deleted.
	 * @param name name of the person to be deleted.
	 */
	public void setPersonToDelete(String name) {
		this.personToDelete = name;
	}

	/**
	 * Getter for the person to be deleted.
	 * @return name of the person to be deleted.
	 */
	public String getPersonToDelete() {
		return this.personToDelete;
	}

	/**
	 * Setter for the boolean indicating if just
	 * one person should be deleted.
	 * @param b boolean for the setter.
	 */
	public void setToDelete(boolean b) {
		this.toDelete = b;
	}

	/**
	 * Getter for the boolean indicating if just
	 * one person should be deleted.
	 * @return the boolean for the getter.
	 */
	public boolean getToDelete() {
		return this.toDelete;
	}

	/**
	 * Inserts an Entity in the tree.
	 * @param a the entity to be added
	 * @param priority the priority which the entity is added with
	 */
	public void insert(Entity a, int priority) {
		this.heap.insert(a, priority);
	}

	/**
	 * Extracts the root from the heap.
	 */
	public void embark() {
		this.heap.extract();
	}

	/**
	 * Lists the max heap in preorder.
	 * @throws IOException possible file error.
	 */
	public void list() throws IOException {
		StringBuilder str = new StringBuilder();
		this.heap.print(0, this.heap.size(), str);
		str.setLength(str.length() - 1);
//		System.out.print(str + "\n");
		
		if(this.firstList == true) {
			this.firstList = false;
			this.fw.append(str);			
		} else {
			this.fw.append("\n" + str);
		}
	}

	/**
	 * Deletes an entire Entity.
	 * @param a the entity to be deleted
	 */
	public void delete(Entity a) {
		if(this.toDelete == true) {
			this.heap.deleteOne(a, personToDelete);
		} else {
			this.heap.delete(a);
		}
	}
}
