##############################################################################################
## Exemplul 1.

# from threading import Thread
 
# def my_concurrent_code(nr, msg):
#     """ Functie care va fi rulata concurent """
#     print ("Thread", nr, "says:", msg)
 
# # creeaza obiectele corespunzatoare thread-urilor
# t1 = Thread(target = my_concurrent_code, args = (1, "hello from thread"))
# t2 = Thread(target = my_concurrent_code, args = (2, "hello from other thread"))
 
# # porneste thread-urile
# t1.start()
# t2.start()
 
# # executia thread-ului principal continua de asemenea
# print ("Main thread says: hello from main")
 
# # asteapta terminarea thread-urilor
# t1.join()
# t2.join()

##############################################################################################
## Exemplul 2.

# from threading import Thread
 
# class MyThread(Thread):
#     """ Clasa care incapsuleaza codul nostru concurent """
#     def __init__(self, nr, msg):
#         Thread.__init__(self)
#         self.nr = nr
#         self.msg = msg
 
#     def run(self):
#         print ("Thread", self.nr, "says:", self.msg)
 
# # creeaza obiectele corespunzatoare thread-urilor
# t1 = MyThread(1, "hello from thread")
# t2 = MyThread(2, "hello from other thread")
 
# # porneste thread-urile
# t1.start()
# t2.start()
 
# # executia thread-ului principal continua de asemenea
# print ("Main thread says: hello from main")

##############################################################################################
## Exemplul 3.

# from threading import Lock, Thread
 
# def inc(lista, lock, index, n):
#     """ Incrementeaza elementul index din lista de n ori """
#     for i in range(n):
#         lock.acquire()
#         lista[index] += 1
#         lock.release()
 
# def dec(lista, lock, index, n):
#     """ Decrementeaza elementul index din lista de n ori """
#     for i in range(n):
#         lock.acquire()
#         lista[index] -= 1
#         lock.release()
# # lista si lock-ul care o protejeaza
# my_list = [0,1,2,3]
# my_lock = Lock()
 
# # thread-urile care modifica elemente din lista
# t1 = Thread(target = inc, args = (my_list, my_lock, 0, 100))
# t2 = Thread(target = dec, args = (my_list, my_lock, 0, 100))
 
# # lista inainte de modificari
# print (my_list)
 
# t1.start()
# t2.start()
 
# t1.join()
# t2.join()
 
# # lista dupa modificari
# print (my_list)

##############################################################################################
## Exemplul 4.

# from threading import RLock, Thread
 
# def inc(lista, rlock, index, n):
#     """ Incrementeaza elementul index din lista de n ori """
#     rlock.acquire()
#     if n > 0:
#         lista[index] += 1               # incrementeaza o data
#         inc(lista, rlock, index, n - 1) # incrementeaza recursiv de n-1 ori
#     rlock.release()
 
# def dec(lista, rlock, index, n):
#     """ Decrementeaza elementul index din lista de n ori """
#     rlock.acquire()
#     if n > 0:
#         lista[index] -= 1               # decrementeaza o data
#         dec(lista, rlock, index, n - 1) # decrementeaza recursiv de n-1 ori
#     rlock.release()
 
# # lista si lock-ul care o protejeaza
# my_list = [0]
# my_lock = RLock()
 
# # thread-urile care modifica elemente din lista
# t1 = Thread(target = inc, args = (my_list, my_lock, 0, 100))
# t2 = Thread(target = dec, args = (my_list, my_lock, 0, 100))
 
# # lista inainte de modificari
# print (my_list)
 
# t1.start()
# t2.start()
 
# t1.join()
# t2.join()
 
# # lista dupa modificari
# print (my_list)

##############################################################################################
## Exemplul 5.

from random import randint, seed
from threading import Semaphore, Thread
from time import sleep
 
def access(nr, sem):
    sem.acquire()
    print ("Thread-ul", nr, " acceseaza")
    sleep(randint(1, 4))
    print ("Thread-ul", nr, " a terminat")
    sem.release()
 
# initializam semaforul cu 3 pentru a avea maxim 3 thread-uri active la un moment dat
semafor = Semaphore(value = 3)
 
# stocam obiectele Thread pentru a putea face join
thread_list = []
 
seed() # seed-ul este current system time pentru generatorul de nr random
 
# pornim thread-urile
for i in range(10):
    thread = Thread(target = access, args = (i, semafor))
    thread.start()
    thread_list.append(thread)
 
# asteptam terminarea thread-urilor
for i in range(len(thread_list)):
    thread_list[i].join()

##############################################################################################
##############################################################################################

