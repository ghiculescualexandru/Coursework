"""
This module represents the Consumer.

Computer Systems Architecture Course
Assignment 1
March 2021
"""

from threading import Thread
from time import sleep

ADD = "add"
REMOVE = "remove"
TYPE = "type"
PRODUCT = "product"
QUANTITY = 'quantity'
NAME = "name"
CONSUMER_MSG = "{} bought {}" # Expects name and item.

class Consumer(Thread):
    """
    Class that represents a consumer.
    """

    def __init__(self, carts, marketplace, retry_wait_time, **kwargs):
        """
        Constructor.

        :type carts: List
        :param carts: a list of add and remove operations

        :type marketplace: Marketplace
        :param marketplace: a reference to the marketplace

        :type retry_wait_time: Time
        :param retry_wait_time: the number of seconds that a producer must wait
        until the Marketplace becomes available

        :type kwargs:
        :param kwargs: other arguments that are passed to the Thread's __init__()
        """
        Thread.__init__(self)
        self.carts = carts
        self.marketplace = marketplace
        self.retry_wait_time = retry_wait_time
        self.kwargs = kwargs

    def run(self):
        for cart in self.carts:
            cart_id = self.marketplace.new_cart()
            for command in cart:
                command_type = command[TYPE]
                product = command[PRODUCT]
                quantity = int(command[QUANTITY])

                if command_type == ADD:
                    while quantity > 0:
                        added = self.marketplace.add_to_cart(cart_id, product)
                        if not added:
                            sleep(self.retry_wait_time)
                            continue

                        quantity -= 1
                elif command_type == REMOVE:
                    while quantity > 0:
                        self.marketplace.remove_from_cart(cart_id, product)
                        quantity -= 1

            items = self.marketplace.place_order(cart_id)
            for item in items:
                print(CONSUMER_MSG.format(self.kwargs[NAME], item))
