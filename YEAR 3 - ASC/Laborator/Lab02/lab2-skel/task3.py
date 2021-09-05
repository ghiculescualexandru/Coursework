"""
Coffee Factory: A multiple producer - multiple consumer approach

Generate a base class Coffee which knows only the coffee name
Create the Espresso, Americano and Cappuccino classes which inherit the base class knowing that
each coffee type has a predetermined size.
Each of these classes have a get message method

Create 3 additional classes as following:
    * Distributor - A shared space where the producers puts coffees and the consumers takes them
    * CoffeeFactory - An infinite loop, which always sends coffees to the distributor
    * User - Another infinite loop, which always takes coffees from the distributor

The scope of this exercise is to correctly use threads, classes and synchronization objects.
The size of the coffee (ex. small, medium, large) is chosen randomly everytime.
The coffee type is chosen randomly everytime.

Example of output:

Consumer 65 consumed espresso
Factory 7 produced a nice small espresso
Consumer 87 consumed cappuccino
Factory 9 produced an italian medium cappuccino
Consumer 90 consumed americano
Consumer 84 consumed espresso
Factory 8 produced a strong medium americano
Consumer 135 consumed cappuccino
Consumer 94 consumed americano
"""

from random import randint
from threading import Semaphore, Lock, Thread

class Coffee:
    """ Base class """
    def __init__(self):
        pass

    def getName(self):
        """ Returns the coffee name """
        raise NotImplementedError

    def getSize(self):
        """ Returns the coffee size """
        raise NotImplementedError


class ExampleCoffee:
    """ Espresso implementation """
    def __init__(self, size):
        pass

    def getMessage(self):
        """ Output message """
        raise NotImplementedError

class Espresso(Coffee):
    def __init__(self):
        Coffee.__init__(self)
        self.__name = "Espresso"
        self.__size = 10

    def getName(self):
        return self.__name

    def getSize(self):
        return self.__size

    def getMessage(self):
        return "some espresso depresso"

class Americano(Coffee):
    def __init__(self):
        Coffee.__init__(self)
        self.__name = "Americano"
        self.__size = 15

    def getName(self):
        return self.__name

    def getSize(self):
        return self.__size

    def getMessage(self):
        return "some americano"

class Cappuccino(Coffee):
    def __init__(self):
        Coffee.__init__(self)
        self.__name = "Cappuccino"
        self.__size = 20

    def getName(self):
        return self.__name

    def getSize(self):
        return self.__size

    def getMessage(self):
        return "hmm cappuccino"

class Distributor():
    def __init__(self, _coffeesNumber):
        self.coffeesNumber = _coffeesNumber
        self.coffeesList = []
        self.v = Semaphore(value = _coffeesNumber)
        # semaphore producers must acquire 
        # to make sure they don't produce more than
        # the buffer size
        self.f = Semaphore(value = 0)
        # semaphore consumers must acquire
        # to make sure they don't consume when
        # buffer is empty
        self.lock = Lock()

class CoffeeFactory(Thread):
    def __init__(self, _id, _distributor):
        Thread.__init__(self)
        self.distributor = _distributor
        self.id = _id

    def makeCoffee(self):
        randNo = randint(1,3)
        if randNo == 1:
            return Cappuccino()
        elif randNo == 2:
            return Americano()
        else:
            return Espresso()

    def run(self):
        while True:
            self.distributor.v.acquire()
            self.distributor.lock.acquire()

            coffee = self.makeCoffee()
            self.distributor.coffeesList.append(coffee)
            print("Factory {} produced {}.".format(self.id, coffee.getMessage()))

            self.distributor.lock.release()
            self.distributor.f.release()


class User(Thread):
    def __init__(self, _id, _distributor):
        Thread.__init__(self)
        self.distributor = _distributor
        self.id = _id

    def consumeCoffee(self):
        return self.distributor.coffeesList.pop(0)

    def run(self):
        while True:
            self.distributor.f.acquire()
            self.distributor.lock.acquire()

            coffee = self.consumeCoffee()
            print("Consumer {} consume {}.".format(self.id, coffee.getMessage()))

            self.distributor.lock.release()
            self.distributor.v.release()


if __name__ == '__main__':
    bufferSize = 10
    N = 10 # N consumers and N producers

    distributor = Distributor(bufferSize)

    for i in range (0, N):
        coffeeFactory = CoffeeFactory(i, distributor)
        coffeeFactory.start()
        user = User(i, distributor)
        user.start()

    for i in range (0, producersNo):
        coffeeFactory.join()

    for i in range (0, consumersNo):
        user.join()