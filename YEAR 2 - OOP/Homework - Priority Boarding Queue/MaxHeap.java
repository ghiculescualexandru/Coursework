import java.util.Vector;
/**
 * Basic Max Heap Tree implemented with Vector.
 * @author Ghiculescu Alexandru
 */
public class MaxHeap {

	private Vector<Entity> arr;	/* array of Entity for the Max Heap Tree */

	/**
	 * Default constructor.
	 */
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public MaxHeap() {
		arr = new Vector();
	}

	/**
	 * Constructor with a given size.
	 * @param capacity The capacity of the heap.
	 */
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public MaxHeap(int capacity) {
		arr = new Vector(capacity);
	}

	/**
	 * Returns the parent of index i.
	 * @param i the index
	 * @return the parent of index i
	 */
	private int parent(int i) {
		if (i == 0) {
			return 0;
		}
		return (i - 1) / 2;
	}

	/**
	 * Returns the left child of index i.
	 * @param i the index
	 * @return the left child of index i
	 */
	private int leftChild(int i) {
		return (2 * i + 1);
	}

	/**
	 * Returns the right child of index i.
	 * @param i the index
	 * @return the right child of index i
	 */
	private int rightChild(int i) {
		return (2 * i + 2);
	}

	/**
	 * Swaps two Entity in the heap.
	 */
	void swap(int a, int b) {
		Entity temp = arr.get(a);
		arr.setElementAt(arr.get(b), a);
		arr.setElementAt(temp, b);
	}

	/**
	 * Solve the max heap condition starting from index i
	 * and going down on the tree.
	 * @param i the index for heapify down to begin
	 */
	private void heapifyDown(int i) {
		int left = leftChild(i);
		int right = rightChild(i);

		int largest = i;

		if (left < size() && arr.get(left).getFinalPriority() > arr.get(largest).getFinalPriority()) {
			largest = left;
		}

		if (right < size() && arr.get(right).getFinalPriority() > arr.get(largest).getFinalPriority()) {
			largest = right;
		}

		if (largest != i) {
			swap(i, largest);

			heapifyDown(largest);
		}
	}

	/**
	 * Solve the max heap condition starting from index i
	 * and going up on the tree.
	 * @param i the index for heapify up to begin
	 */
	private void heapifyUp(int i) {
		if (i > 0 && i < arr.size() && arr.get(parent(i)).getFinalPriority() < arr.get(i).getFinalPriority()) {
			swap(i, parent(i));

			heapifyUp(parent(i));
		}
	}

	/**
	 * @return the heap size
	 */
	public int size() {
		return arr.size();
	}

	/**
	 * Inserts an Entity in the heap and heapify
	 * the tree from the index where it was added.
	 * @param a the Entity to be added
	 * @param priority the priority which the Entity
	 * 		  is inserted with
	 */
	public void insert(Entity a, int priority) {
		a.setFinalPriority(priority);
		arr.addElement(a);

		int index = size() - 1;
		heapifyUp(index);
	}

	/**
	 * Deletes an Entity from the heap and heapify 
	 * the tree up and down from the index where
	 * it was deleted.
	 * @param a the entity to be deleted
	 */
	public void delete(Entity a) {
		int i;
		for (i = 0; i < arr.size(); i++) {
			if (arr.get(i).getID().equals(a.getID()) == true) {
				arr.setElementAt(arr.lastElement(), i);
				arr.remove(arr.size() - 1);
				break;
			}
		}
		
		heapifyUp(i);
		heapifyDown(i);
	}

	
	/**
	 * Deletes a person from an Entity.
	 * @param a the Entity containing the person to be deleted
	 * @param name the person to be deleted
	 */
	public void deleteOne(Entity a, String name) {
		int i;
		for (i = 0; i < this.size(); i++) {
			if(arr.get(i) == a) {	
				if (arr.get(i) instanceof Family) {
					((Family) arr.get(i)).deleteMember(name);
				} else if (arr.get(i) instanceof Group) {
					((Group) arr.get(i)).deleteMember(name);
				}

				a.setFinalPriority(a.getPriority());

				break;
			}
		}

		heapifyDown(i);
	}

	
	/**
	 * Extracts the root and heapify the tree.
	 * @return the root
	 */
	public Entity extract() {
		Entity root = arr.firstElement();
		arr.setElementAt(arr.lastElement(), 0);
		arr.remove(size() - 1);

		heapifyDown(0);

		return root;
	}

	/**
	 * Display the Max Heap Tree in preorder.
	 * @param idx the element index
	 * @param size the heap size
	 */
	public void print(int idx, int size, StringBuilder str) {
		if (idx >= size)
			return;

		str.append(arr.get(idx).getID() + " ");
		print(idx * 2 + 1, size, str);
		print(idx * 2 + 2, size, str);
	}
}
