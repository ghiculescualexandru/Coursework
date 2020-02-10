JFLAGS = -g
JC = javac
JVM= java
FILE=

.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	Entity.java \
	Family.java \
	Group.java \
	MaxHeap.java \
	Person.java \
	PriorityBoarding.java \
	PriorityQueue.java \
	Single.java \
	SpecialNeeds.java \
	Ticket.java \
	ReadMain.java

MAIN = ReadMain

default: CLASSES

classes: $(CLASSES:.java=.class)

run: $(MAIN).class
	$(JVM) $(MAIN)

clean:
	$(RM) *.class