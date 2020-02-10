JFLAGS = -g
JC = javac
JVM= java
FILE=

.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	House.java \
	Device.java \
	HumidityObservation.java \
	TemperatureObservation.java \
	Room.java \
	Main.java

MAIN = Main

default: CLASSES

classes: $(CLASSES:.java=.class)

run: $(MAIN).class
	$(JVM) $(MAIN)

clean:
	$(RM) *.class
