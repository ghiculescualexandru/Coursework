from threading import Lock, Thread
import time

FIRST_TO_EAT = 0

class Philospher(Thread):
	def __init__(self, _id, _leftFork, _rightFork):
		Thread.__init__(self)
		self.id = _id
		self.leftFork = _leftFork
		self.rightFork = _rightFork

	def tryingToEat(self):
		print("Philospher {} is trying to eat...".format(self.id))
		time.sleep(1)

	def eat(self):
		print("Philospher {} is eating...".format(self.id))

	def run(self):
		if self.id == FIRST_TO_EAT:
			self.rightFork.acquire()
			self.tryingToEat()			
			self.leftFork.acquire()
			self.eat()
			self.leftFork.release()
			self.rightFork.release()
		else:
			self.leftFork.acquire()
			self.tryingToEat()			
			self.rightFork.acquire()
			self.eat()
			self.rightFork.release()
			self.leftFork.release()

if __name__ == '__main__':
	philosophersNumber = 5
	philosophers = [None] * philosophersNumber
	forks = [None] * philosophersNumber

	for i in range(0, philosophersNumber):
		forks[i] = Lock()

	for i in range(0, philosophersNumber):
		philosophers[i] = Philospher(i, forks[i], forks[(i + 1) % philosophersNumber])

	for i in range(0, philosophersNumber):
		philosophers[i].start()

	for i in range(0, philosophersNumber):
		philosophers[i].join()
