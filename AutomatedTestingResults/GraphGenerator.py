import matplotlib
import os

if __name__ == "__main__":
    if os.path.exists("./CProgramResultsType0.txt") == True:
        fp = open("./CProgramResultsType0.txt", "r")

        Lines = fp.readlines()

        for line in Lines:
            print(line[:-1])

            #for first line
            