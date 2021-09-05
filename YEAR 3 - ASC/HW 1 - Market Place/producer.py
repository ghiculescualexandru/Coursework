"""
This module represents the Producer.

Computer Systems Architecture Course
Assignment 1
March 2021
"""

from threading import Thread
from time import sleep

class Producer(Thread):
    """
    Class that represents a producer.
    """
    def __init__(self, products, marketplace, republish_wait_time, **kwargs):
        """
        Constructor.

        @type products: List()
        @param products: a list of products that the producer will produce

        @type marketplace: Marketplace
        @param marketplace: a reference to the marketplace

        @type republish_wait_time: Time
        @param republish_wait_time: the number of seconds that a producer must
        wait until the marketplace becomes available

        @type kwargs:
        @param kwargs: other arguments that are passed to the Thread's __init__()
        """
        Thread.__init__(self, daemon=kwargs["daemon"])
        self.products = products
        self.marketplace = marketplace
        self.republish_wait_time = republish_wait_time
        self.kwargs = kwargs

    def run(self):
        producer_id = self.marketplace.register_producer()

        while True:
            for product in self.products:
                product_to_publish = product[0]
                quantity = product[1]
                producing_time = product[2]

                sleep(producing_time)

                while quantity > 0:
                    produced = self.marketplace.publish(producer_id, product_to_publish)
                    if not produced:
                        sleep(self.republish_wait_time)
                        continue

                    quantity -= 1
