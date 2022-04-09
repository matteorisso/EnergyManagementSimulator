# Description: Reads trace.dat file and saves the measures into Matlab-wise vectors.
# Author: Antonio De Luca
# Date: 2022-04-03
#
# Comments: it is a mess, but it works

from pickle import LIST
import pickle
import sys
import string
import os

list_of_lists = []
dictionary = {}

with open("trace.dat", "r") as fileobj:
    firstLine = fileobj.readline()
    firstLine = firstLine[1:]
    firstLineList = firstLine.split()

    sizeList = len(firstLineList)
    for i in range(sizeList):
        newList = []
        list_of_lists.append(newList)

    line = fileobj.readline()       # starting from the second line
    while line:
        lineList = line.split()
        for i in range(sizeList):
            list_of_lists[i].append(lineList[i])
        line = fileobj.readline()
    
    fileobj.close()

    for i in range(sizeList):
        dictionary[firstLineList[i]] = list_of_lists[i]

with open("trace.m", "w") as fileobjWrite:
    sys.stdout = fileobjWrite
    for key in dictionary.keys():
        print(key, "=", dictionary[key], sep="", end=";\n\n")
    sys.stdout = sys.__stdout__
    fileobj.close()

with open("trace.m", "r") as fileobjRead:
    with open("traces.m", "w") as fileobjWrite:
        for line in fileobjRead:
            newline = line.replace("'", "")
            fileobjWrite.write(newline)
    fileobjRead.close()
    fileobjWrite.close()

os.remove("trace.m")