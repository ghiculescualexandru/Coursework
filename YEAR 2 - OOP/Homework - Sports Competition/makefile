JFLAGS = -g
JC = javac
JVM= java
FILE=

.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	Player.java \
	Team.java \
	TeamFactory.java \
	BasketballTeam.java \
	FootballTeam.java \
	HandballTeam.java \
	CreateTeams.java \
	Main.java

MAIN = Main

default: CLASSES

classes: $(CLASSES:.java=.class)

run: $(MAIN).class
	$(JVM) $(MAIN) $(COMANDA) $(IN1) $(IN2) $(OUT)

clean:
	$(RM) *.class
