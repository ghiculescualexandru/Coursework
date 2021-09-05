"""
This module represents the Marketplace.

Computer Systems Architecture Course
Assignment 1
March 2021
"""

from threading import Lock

class Marketplace:
    """
    Class that represents the Marketplace. It's the central part of the implementation.
    The producers and consumers use its methods concurrently.
    """
    def __init__(self, queue_size_per_producer):
        """
        Constructor

        :type queue_size_per_producer: Int
        :param queue_size_per_producer: the maximum size of a queue associated with each producer
        """
        self.queue_size_per_producer = queue_size_per_producer
        self.queues = dict()
        self.carts = dict()
        self.producers_no = 0
        self.carts_no = 0
        self.marketplace_lock = Lock()
        self.producer_lock = Lock()
        self.cart_lock = Lock()

    def register_producer(self):
        """
        Returns an id for the producer that calls this.
        """

        # Using the lock so not multiple producers get the same id.
        self.producer_lock.acquire()
        try:
            producer_id = self.producers_no
            self.queues[producer_id] = list()
            self.producers_no += 1
        finally:
            self.producer_lock.release()

        return producer_id

    def publish(self, producer_id, product):
        """
        Adds the product provided by the producer to the marketplace

        :type producer_id: String
        :param producer_id: producer id

        :type product: Product
        :param product: the Product that will be published in the Marketplace

        :returns True or False. If the caller receives False, it should wait and then try again.
        """
        if len(self.queues[producer_id]) >= self.queue_size_per_producer:
            return False

        print("publishing")

        self.queues[producer_id].append(product)
        return True

    def new_cart(self):
        """
        Creates a new cart for the consumer

        :returns an int representing the cart_id
        """

        # Using the lock so not multiple consumers get the same cart id.
        self.cart_lock.acquire()
        try:
            cart_id = self.carts_no
            self.carts[cart_id] = list()
            self.carts_no += 1
        finally:
            self.cart_lock.release()

        return cart_id

    def add_to_cart(self, cart_id, product):
        """
        Adds a product to the given cart.

        :type cart_id: Int
        :param cart_id: id cart

        :type product: Product
        :param product: the product to add to cart

        :returns True or False. If the caller receives False, it should wait and then try again
        """

        print("adding to cart")

        self.marketplace_lock.acquire()
        # Search for the product in all producers queues.
        for producer, products in self.queues.items():
            for prod in products:
                if prod.name == product.name:
                    self.queues[producer].remove(prod)
                    # Put in cart both the product and the producer's id.
                    self.carts[cart_id].append((prod, producer))

                    self.marketplace_lock.release()
                    return True

        print("can't add to cart " + str(product))
        print(self.queues)

        self.marketplace_lock.release()
        return False

    def remove_from_cart(self, cart_id, product):
        """
        Removes a product from cart.

        :type cart_id: Int
        :param cart_id: id cart

        :type product: Product
        :param product: the product to remove from cart
        """

        cart = self.carts[cart_id]

        print("removing from cart")

        self.marketplace_lock.acquire()
        for item in cart:
            cart_product = item[0]
            producer = item[1]

            if cart_product.name == product.name:
                # Put the product back in the queue from
                # which it was taken in the first place.
                self.queues[producer].append(product)
                cart.remove(item)
                break

        self.marketplace_lock.release()

    def place_order(self, cart_id):
        """
        Return a list with all the products in the cart.

        :type cart_id: Int
        :param cart_id: id cart
        """
        items = list()
        for item in self.carts[cart_id]:
            # Only return the items, not the producer
            # id from which queue the products were taken.
            items.append(item[0])

        return items
