"""
A command-line controlled coffee maker.
"""

import sys
import load_recipes

"""
Implement the coffee maker's commands. Interact with the user via stdin and print to stdout.

Requirements:
    - use functions
    - use __main__ code block
    - access and modify dicts and/or lists
    - use at least once some string formatting (e.g. functions such as strip(), lower(),
    format()) and types of printing (e.g. "%s %s" % tuple(["a", "b"]) prints "a b"
    - BONUS: read the coffee recipes from a file, put the file-handling code in another module
    and import it (see the recipes/ folder)

There's a section in the lab with syntax and examples for each requirement.

Feel free to define more commands, other coffee types, more resources if you'd like and have time.
"""

"""
Tips:
*  Start by showing a message to the user to enter a command, remove our initial messages
*  Keep types of available coffees in a data structure such as a list or dict
e.g. a dict with coffee name as a key and another dict with resource mappings (resource:percent)
as value
"""

# Commands
EXIT = "exit"
LIST_COFFEES = "list"
MAKE_COFFEE = "make"  #!!! when making coffee you must first check that you have enough resources!
HELP = "help"
REFILL = "refill"
RESOURCE_STATUS = "status"
COMMANDS = [EXIT, LIST_COFFEES, MAKE_COFFEE, REFILL, RESOURCE_STATUS, HELP]

# Resources examples
WATER = "water"
COFFEE = "coffee"
MILK = "milk"

# Coffee examples
ESPRESSO = "espresso"
AMERICANO = "americano"
CAPPUCCINO = "cappuccino"
AVAILABLE_COFFEES = load_recipes.AVAILABLE_COFFEES
COFFES_RESOURCES = load_recipes.COFFES_RESOURCES

# Coffee maker's resources - the values represent the fill percents
RESOURCES = { WATER: 100, COFFEE: 100, MILK: 100 }

# Other constant strings
COFFEE_NOT_AVAILABLE = "coffee_not_available"
ACCEPTED_COMMANDS = "accepted_commands"
YOUR_COFFEE = "your_coffee"
RESOURCE_NOT_AVAILABLE = "resource_not_available"

# Text messages
messages = {
    ACCEPTED_COMMANDS: 'Accepted commands: ',
    LIST_COFFEES: 'Here are my available coffees: ',
    MAKE_COFFEE: 'Which coffee would you like?',
    YOUR_COFFEE: 'Here\'s your {}!',
    RESOURCE_STATUS: 'Here is my status: ',
    REFILL: 'Which resource? Type \'all\' for refilling everything',
    COFFEE_NOT_AVAILABLE: 'This coffee is not available.',
    RESOURCE_NOT_AVAILABLE: 'This resource is not used.'
}

"""
Example result/interactions:

I'm a smart coffee maker
Enter command:
list
americano, cappuccino, espresso
Enter command:
status
water: 100%
coffee: 100%
milk: 100%
Enter command:
make
Which coffee?
espresso
Here's your espresso!
Enter command:
refill
Which resource? Type 'all' for refilling everything
water
water: 100%
coffee: 90%
milk: 100%
Enter command:
exit
"""

def display_instructions():
    print(messages[ACCEPTED_COMMANDS])
    for command in COMMANDS:
        print(" - " + command)

def list_coffees():
    print(messages[LIST_COFFEES])
    for coffee in AVAILABLE_COFFEES:
        print(" - " + coffee)

def make_coffee():
    print(messages[MAKE_COFFEE])
    coffee = sys.stdin.readline().strip()
    if coffee in AVAILABLE_COFFEES:
        for (resource, percent) in COFFES_RESOURCES[coffee].items():
            RESOURCES[resource] -= percent
        print(messages[YOUR_COFFEE].format(coffee))
    else:
        print(messages[COFFEE_NOT_AVAILABLE])
        list_coffees()

def resource_status(intro_message = True):
    if intro_message:
        print(messages[RESOURCE_STATUS])
    for (resource, percent) in RESOURCES.items():
        print(" - " + resource + ": " + str(percent) + "%") 

def refill():
    print(messages[REFILL])
    refill_resource = sys.stdin.readline().strip()
    if refill_resource == 'all':
        for (resource,_) in RESOURCES.items():
            RESOURCES[resource] = 100
    else:
        if refill_resource in RESOURCES:
            RESOURCES[refill_resource] = 100
        else:
            print(messages[RESOURCE_NOT_AVAILABLE])

    resource_status(intro_message = False)

def handle_command(command):
    if command == LIST_COFFEES:
        list_coffees()
    elif command == MAKE_COFFEE:
        make_coffee()
    elif command == HELP:
        display_instructions()
    elif command == REFILL:
        refill()
    elif command == RESOURCE_STATUS:
        resource_status()

if __name__ == "__main__":
    print("I'm a simple coffee maker")
    print("Here is the instruction manual :)")
    display_instructions()

    while True:
        print("Enter command: ")
        command = sys.stdin.readline().strip() # Using string formatting.

        if command not in COMMANDS:
            display_instructions()
            continue

        if command == EXIT:
            break
        
        handle_command(command)

    print("Teach me how to make coffee...please...byezzz")
