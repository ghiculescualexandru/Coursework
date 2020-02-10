
GHICULESCU-ALEXANDRU-IONUT, 325-CB,  COURSEWORK 2 - POO

---------------------------------------------------------------------------------------------------------------------------------

	This java program simulates a heat station for a house with a given number of rooms.
	
---------------------------------------------------------------------------------------------------------------------------------
	
	The "House" HAS An array of "Room"s and other details consisting of:
		- its global temperature desired (double)
		- its global humidity desired (double)
		- its time stamp of reference (long)
	
	It has basic methods, such as adding a room, adding a temperature or humidity observation,
	listing the temperature observations in a given interval and the Trigger Heat command. 
		- the Trigger Heat method computes the weighted average of the temperature and humidity
		  (if a global humidity has been previously set). The weighted average depends on
		  the area of the room. If a room does not have a temperature or humidity
		  observation registered yet, the method does not take into consideration that
		  room or the area of that room.
		- the other methods are called for a given room (will talk about them for the room class).
		
---------------------------------------------------------------------------------------------------------------------------------
	
	The "Room" HAS A "Device" and other details consisting of:
		- its area (double)
		- its name (string, eg: ROOM1)
		- its device ID (string)
		
	It has basic methods, such as adding a temperature or humidity observation, listing the
	temperature observations in a given interval, getting the minimum temperature registered
	in the last hour or the maximum humidity registered in the last hour. Every single method
	of the ones mentioned above are called in the Room's device as we will see next.
	
---------------------------------------------------------------------------------------------------------------------------------
	
	The "Device" is the most important class and its details consists of:
		- it HAS An ArrayList of TreeSet of TemperatureObservation 
		- it HAS An ArrayList of TreeSet of HumidityObservation
		- the room name (string)
		- the device id (string)
		
	Given the fact that our heat station works in time series given by the reference time stamp, 
	I divided the observations in 24 lists, each list having a TreeSet of Observations.
	
	Basically, if a temperature/humidity is registered 2500 seconds ago, I will add this
	in the TreeSet for the first ArrayList (index starts from 0), so the ArrayList.get(0).
	If a temperature/humidity is registered 3601 seconds ago, it will be added in the 
	TreeSet for the second ArrayList, so the ArrayList.get(1).
	
	The TreeSet has been chosen because its elements are sorted and it has no duplicates, and
	in the Device constructor I override the compare method to order the elements by the
	value of temperature/humidity.
	
	Its methods are the following:
		- add temperature/humidity observations, which basically just add at the given hour
		  in the needed ArrayList, an observation to the TreeSet of that ArrayList
		- listing the temperature observations in a given interval, given by a start hour and
		  a stop hour. This method just iterates through the hours given and converted from
		  time stamps, and for every hour goes to the ArrayList desired, and writes for
		  every element in the TreeSet the temperature.
		- computing the minimum temperature in the last hour registered basically is 
		  going through every ArrayList until finding the first one which is not empty.
		  Then, because a TreeSet is used, the method just returns the first element
		  of the TreeSet
		- computing the maximum humidity in the last hour registered works the same
		  as above, only that the method just returns the last element of the TreeSet
		
---------------------------------------------------------------------------------------------------------------------------------
	
	"TemperatureObservations" and "HumidityObservations" are just structures that hold
	a time stamp and the value of the temperature/humidity for that moment. They have
	basic constructors, getters and setters.

---------------------------------------------------------------------------------------------------------------------------------

	"Main" has a main method in which I basically read the input and call desired
	methods of the house for the actions in the input.
	
---------------------------------------------------------------------------------------------------------------------------------

	*Further observations are displayed in the comment blocks.
	
