"""
    Basic thread handling exercise:

    Use the Thread class to create and run more than 10 threads which print their name and a random
    number they receive as argument. The number of threads must be received from the command line.

    e.g. Hello, I'm Thread-96 and I received the number 42

"""
from random import seed
from random import randint
import sys
from threading import Thread

seed()
THREAD_MSG = "Hello from thread [{}], I got number [{}]."

class MyThread(Thread):
	def __init__(self, _id, _randomNumber):
		Thread.__init__(self);
		self.id = _id
		self.randomNumber = _randomNumber

	def run(self):
		print(THREAD_MSG.format(self.id, self.randomNumber))

if __name__ == '__main__':
	threadsNumber = sys.argv[1]
	print("Number of threads: ", threadsNumber)

	threadsList = []

	for i in range(0, int(threadsNumber)):
		thread = MyThread(i, randint(10, 99))
		thread.start()
		threadsList.append(thread)

	for i in range(0, int(threadsNumber)):
		threadsList[i].join()

	print("Exit...")