"""
    Bonus task: load all the available coffee recipes from the folder 'recipes/'
    File format:
        first line: coffee name
        next lines: resource=percentage

    info and examples for handling files:
        http://cs.curs.pub.ro/wiki/asc/asc:lab1:index#operatii_cu_fisiere
        https://docs.python.org/3/library/io.html
        https://docs.python.org/3/library/os.path.html
"""
import os

COFFES_RESOURCES = {}
AVAILABLE_COFFEES = []

RECIPES_FOLDER = "./recipes"
RECIPES_FILES = os.listdir(RECIPES_FOLDER)

for file in RECIPES_FILES:
    f = open(RECIPES_FOLDER + "/" + file, "r")
    first_line = True

    coffee = ""
    recipe = {}

    for line in f:
        line = line.strip()
        if first_line:
            coffee = line
            AVAILABLE_COFFEES.append(coffee)
            first_line = False
        else:
            aux = line.split("=")
            recipe[aux[0]] = int(aux[1])

    COFFES_RESOURCES[coffee] = recipe