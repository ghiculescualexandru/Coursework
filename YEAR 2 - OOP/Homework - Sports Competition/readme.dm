
GHICULESCU-ALEXANDRU-IONUT, 325-CB,  COURSEWORK 3 - POO

---------------------------------------------------------------------------------

	This java program simulates a sport competition, using Design Patterns.
	
---------------------------------------------------------------------------------
	
	The "Player" has basic attributes(such as name and score) and basic methods
and overrides toString to display all the details of the player. 
		
---------------------------------------------------------------------------------
	
	The "Team" has basic attributes(such as name, gender, score points,
a list with the players, the number of players and the place of the team) and
basic methods. It also has a method to display all the details of the team
as it is desired.
	
---------------------------------------------------------------------------------

	"TeamFactory" uses the design pattern for factory and
it also uses singleton.
	"TeamVisitor" is an interface for the visitor pattern.
	"TeamDisplayVisitor" is a class that implements 
the visitor interface and its methods to compute the 
overall score points for any team.

	"FotballTeam","HandballTeam" and "BasketballTeam"
extend the main "Team" class.
		
---------------------------------------------------------------------------------
	
	The "Competition" has its main attributes such as its type, gender
a list with all teams, an ordered list with the team called "ranking"
a list of observers and basic constructors.
	It has basic methods and also methods for the observer pattern.

---------------------------------------------------------------------------------
	
	The "Observer" class has an abstract method and its basic 
pattern constructors.

---------------------------------------------------------------------------------

	The "CompetitionObserver" simply overrides the update by sorting
the list with the teams and updating the ranking list.

---------------------------------------------------------------------------------

	"CreateTeams" and "CreateCompetition" are designed to basically
handle the reading of the input, basic actions and to display the
output into a specified file.

---------------------------------------------------------------------------------
