
## GHICULESCU-ALEXANDRU-IONUT, 325-CB,  COURSEWORK 1 - POO ## 

---------------------------------------------------------------------------------------------------------------------------------

This java program simulates a priority queue for embarking passangers of a
a flight based on a priority calculated beforehand.

A passanger is an ENTITY. This means a passanger can be either a "Single", 
a "Family" or a "Group". 

	The classes are defined as it follows:
	-"Single" IS A "Entity" and HAS A "Person";
	- "Family" IS A "Entity" and HAS A array of "Person";
	- "Group"  IS A "Entity" and HAS A array of "Person";

	The 3 above implements abstract methods defined in ENTITY: getPriority,printID and getCnt.
	For each:
		-> getPriority 	returns the total priority of one Entity(for single,
					  is just its person priority, for family and group, it sums
					  the priority of all members from its array).
		-> printID	displays the ID of the Entity.
		-> getCnt     	return the number of persons one Entity has(for single 
					  it just returns 1 and for group or family it returns
					  the number of members).

	In addition, Family and Group have two methods which add and delete one member from it.

	- "Person" has main attributes of one person, such as: age, name, ticket,
	its priority boarding and special needs. In "getPriority" method, it returns
	the total priority of one person, calculated by different criterias: its age, 
	its type of ticket(economy, business or premium), if it has priority boarding or
	special needs, it gets different bonus points(boarding = 30,needs = 100).

	- "Ticket" has just a char attribute that holds the type of the ticket, 
	which generates the Ticket priority.
	- "PriorityBoarding" and "SpecialNeeds" have just a boolean that holds 
	whether one person has or not priority boarding or special needs.

---------------------------------------------------------------------------------------------------------------------------------

	- "MaxHeap" HAS A vector of Entity which will be shaped into a Max Heap Tree
	every time an action is done to the vector such as: insert Entity, delete Entity,
	delete the root or delete a member of an Entity.
	  It has usual methods such as HeapifyUp and HeapifyDown which maintain the
	Max Heap structure of the tree. Extract deletes the root, put in its place the
	last node and then do the heapify. Heapify is also used when an Entity is
	deleted. When just one member is deleted from an Entity, the Entity's priority 
	is updated, the Entity is deleted from the heap and re-inserted back in, 
	heapifying the tree in the process.
	  Finally, the MaxHeap class has a print method which displays the tree in
	preorder(Root-L-R).

	- "PriorityQueue" HAS A MaxHeap, a FileWriter and a boolean for the first
	list. Its basic constructor sets the empty MaxHeap of a given capacity and the
	filewriter used for printing in the file.
	  Its methods call the MaxHeap methods as it follows:
	  	-> insert: inserts an Entity in the MaxHeap;
	  	-> embark: extracts the root and heapify the tree;
	  	-> delete: deletes and Entity from the tree and heapify it;
	  	-> deleteOne: deletes a member from the a family or group;
	  	-> list: display the tree in preorder, using the firstList boolean for
	a clean writing in the output file.

---------------------------------------------------------------------------------------------------------------------------------

	- "ReadMain" has two methods and the main. The two methods: reads every
	person details, create a person and insert it in the right Entity by using a 
	hashMap and reads every action to be done.
	  In the main method, two files are open: input and output and the method
	readActions is called with the hashMap returned by readACtions as a paramater.

---------------------------------------------------------------------------------------------------------------------------------

	*Further observations are displayed in the comment blocks.

