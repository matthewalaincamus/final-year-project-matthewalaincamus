import os
from os.path import exists

import ListGenerator
import SpellChecker

def mainMenu():

    #used to check if the user wants to exit the system
    returnCheck = 1

    #loop till user exits system
    while returnCheck == 1:
        #clear screen to reduce clutter
        os.system("cls")

        #introduce the system
        print("***Welcome to Matthew Camus's spell checking software in Python***")
        print("")

        #show user options that they can do
        print("options to choose from (indicate which option by typing its asscociated letter):")
        print("(A) Generate a dataset from the Corpus")
        print("(B) perform spell checking")
        print("(C) automatic testing of whole system")
        print("(Z) Exit system")
        print("\n")
    
        #check to see if user input is valid
        inputCheck = 1

        while inputCheck == 1:
            ActionChoice = input(": ")

            if ActionChoice == "A" or ActionChoice == "a":
                errorCheck = GenerateDataset()

                #if -1, error occured
                #if 0, reload the page
                if errorCheck == 0:
                    inputCheck = 2
                elif errorCheck == -1:
                    inputCheck = -1

            elif ActionChoice == "B" or ActionChoice == "b":
                errorCheck = TypeSelector()

                #if -1, error occured
                #if 0, reload the page
                if errorCheck == 0:
                    inputCheck = 2
                elif errorCheck == -1:
                    inputCheck = -1

            elif ActionChoice == "C" or ActionChoice == "c":
                errorCheck = AutoTesterMenu()

                #if -1, error occured
                #if 0, reload the page
                if errorCheck == 0:
                    inputCheck = 2
                elif errorCheck == -1:
                    inputCheck = -1

            elif ActionChoice == "Z" or ActionChoice == "z":
                inputCheck = 0

                #remove all files to save storage space
                try:
                    os.remove("./GeneratedFiles/LWD.txt")
                    os.remove("./GeneratedFiles/LWDS.txt")
                    os.remove("./GeneratedFiles/LWND.txt")
                    os.remove("./GeneratedFiles/LWNDS.txt")
                except FileNotFoundError:
                    continue

            else:
                print("Invalid response, please try again")

        if inputCheck == 0:
            returnCheck = 0
        elif inputCheck == -1:
            returnCheck = -1

    return returnCheck

#function for dealing with all things regarding the generation of a word file
def GenerateDataset():

    #check if the user wishes to return to the main menu
    returnCheck = 1

    while returnCheck == 1:
        os.system("cls")

        #show user options for the corpus
        print("options for corpus (indicate which option by typing its asscociated letter):")
        print("(A) text into single list with duplicates")
        print("(B) text into single list without duplicates")
        print("(C) text into single list with duplicates and sorted alphabeticaly")
        print("(D) text into single list without duplicates and sorted alphabeticaly (merge sort)")
        print("(Z) return to main menu\n")

        #check the validity of user input
        inputCheck = 1

        while inputCheck == 1:
            #get user input
            ActionChoice = input(": ")

            if ActionChoice == "A" or ActionChoice == "a":
                errorCheck = ListGenerator.DataReader(True, False, False)

                #if -1, an error occured, if 0 then reload the page
                if errorCheck == 0:
                    inputCheck = 2
                elif errorCheck == -1:
                    inputCheck = -1

            elif ActionChoice == "B" or ActionChoice == "b":
                errorCheck = ListGenerator.DataReader(False, False, False)

                #if -1, an error occured, if 0 then reload the page
                if errorCheck == 0:
                    inputCheck = 2
                elif errorCheck == -1:
                    inputCheck = -1
            
            elif ActionChoice == "C" or ActionChoice == "c":
                errorCheck = ListGenerator.DataReader(True, True, False)

                #if -1, an error occured, if 0 then reload the page
                if errorCheck == 0:
                    inputCheck = 2
                elif errorCheck == -1:
                    inputCheck = -1

            elif ActionChoice == "D" or ActionChoice == "d":
                errorCheck = ListGenerator.DataReader(False, True, False)

                #if -1, an error occured, if 0 then reload the page
                if errorCheck == 0:
                    inputCheck = 2
                elif errorCheck == -1:
                    inputCheck = -1

            elif ActionChoice == "Z" or ActionChoice == "z":
                inputCheck = 0

            else:
                print("Invalid response, please try again")

        # if inputCheck == 0 that means that the user wishes to leave the system
        # otherwise it means that the user still wishes to do other things with the system or and error has occured (system will loop)
        if inputCheck == 0:
            returnCheck = 0
        elif inputCheck == -1:
            returnCheck = -1

    return returnCheck

def TypeSelector():
    os.system("cls")

    #used to see which files have been generated or not
    FileFlags = [0,0,0,0]

    #get all the files generated
    fileList = os.listdir("./GeneratedFiles")
    
    #if the relevant file is found, update its relevant flag
    if "LWD.txt" in fileList:
        FileFlags[0] = 1
    if "LWND.txt" in fileList:
        FileFlags[1] = 1
    if "LWDS.txt" in fileList:
        FileFlags[2] = 1
    if "LWNDS.txt" in fileList:
        FileFlags[3] = 1

    if FileFlags == [0,0,0,0]:
        print("You will need to generate a word file from the corpus before spell checking is possible")

        #user input
        ActionChoice = input("Press anything to return to the main menu: ")

        return 0

    else:
        print("Please select a wordlist from the options below (type the respective letter)")

        if (FileFlags[0] == 1):
            print("(A) use text with duplicate")
        if (FileFlags[1] == 1):
            print("(B) use text without duplicates")
        if (FileFlags[2] == 1):
            print("(C) use text with duplicates sorted in alphabetical order")
        if (FileFlags[3] == 1):
            print("(D) use text without duplicates sorted in alphabetical order")

        print("(Z) return to main menu")

        #check validity of user input
        inputCheck = 1

        while inputCheck == 1:
            ActionChoice = input(": ")

            if (ActionChoice == "A" or ActionChoice == "a") and FileFlags[0] == 1:
                MethodCheck(0)
                inputCheck = 0

            elif (ActionChoice == "B" or ActionChoice == "b") and FileFlags[1] == 1:
                MethodCheck(1)
                inputCheck = 0

            elif (ActionChoice == "C" or ActionChoice == "c") and FileFlags[2] == 1:
                MethodCheck(2)
                inputCheck = 0

            elif (ActionChoice == "D" or ActionChoice == "d") and FileFlags[3] == 1:
                MethodCheck(3)
                inputCheck = 0
            
            elif ActionChoice == "Z" or ActionChoice == "z":
                inputCheck = 0

            else:
                print("Invalid response, please try again")

    return 0

def MethodCheck(FileType : int):

    #check to see if the user wishes to return to the main menu
    returnCheck = 1

    while returnCheck == 1:
        #clear screen to declutter
        os.system("cls")

        print("**Current Filetype selected: ")

        if FileType == 0: print("text with duplicate")
        elif FileType == 1: print("text without duplicates")
        elif FileType== 2: print("text with duplicates sorted in alphabetical order")
        elif FileType == 3: print("text without duplicates sorted in alphabetical order")

        print("")

        #show user options for the corpus
        print("options for word searching methods (indicate which option by typing its asscociated letter):");
        print("(A) word dictionary as a list checked using linear search")
        print("(B) word dictionary as a list checked using binary search (sorted word list only)")
        print("(C) word dictionary as a hash table")
        print("(Z) return to previous menu")

        #check validity of user input
        inputCheck = 1

        while inputCheck == 1:
            #user input
            ActionChoice = input(": ")

            if ActionChoice == "A" or ActionChoice == "a":
                errorCheck = SpellCheck(FileType, 0)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif (ActionChoice == "B" or ActionChoice == "b") and (FileType == 2 or FileType == 3):
                errorCheck = SpellCheck(FileType, 1)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "C" or ActionChoice == "c":
                errorCheck = SpellCheck(FileType, 2)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "Z" or ActionChoice == "z":
                inputCheck = 0

            elif (ActionChoice == "B" or ActionChoice == "b") and (FileType == 0 or FileType == 1):
                print("the selected file type needs to be sorted to perform this action")

            else:
                print("Invalid response, please try again")
        # if inputCheck == 0 that means that the user wishes to leave the system
        # otherwise it means that the user still wishes to do other things with the system or and error has occured (system will loop)
        if inputCheck == 0: returnCheck = 0
        elif inputCheck == -1: returnCheck = -1

    return returnCheck

def SpellCheck(FileType : int, MethodType : int):

    #check to see if the user wished to return to the main menu
    returnCheck = 1

    while returnCheck == 1:
        #clear the screen to declutter
        os.system("cls")

        print("**Current Filetype selected: ")

        #diplay the filetype
        if FileType == 0: print("text with duplicate")
        elif FileType == 1: print("text without duplicates")
        elif FileType== 2: print("text with duplicates sorted in alphabetical order")
        elif FileType == 3: print("text without duplicates sorted in alphabetical order")

        #display the method used
        if MethodType == 0: print("*Method(s): Word list with linear search")
        elif MethodType == 1: print("*Method(s): Word list with binary search")
        elif MethodType == 2: print("*Method(s): Word list as a hash table")

        print("")

        #show user options for the corpus
        print("options for spell checking algorithms (indicate which option by typing its asscociated letter):")
        print("(A) simple check without suggestions")
        print("(B) simple check followed by suggestions if wrong using Levenshtein Distance")
        print("(C) simple check followed by suggestions if wrong using Hamming Distance")
        print("(D) simple check followed by suggestions if wrong using Sorensen-Dice Coefficient")
        print("(E) simple check followed by suggestions if wrong using Optimal String Alignment Distance")
        print("(F) simple check followed by suggestions if wrong using Damerau-Levenshtein Distance")
        print("(G) simple check followed by suggestions if wrong using Jaro Similarity")
        print("(H) simple check followed by suggestions if wrong using Jaro-Winkler Similarity")
        print("(Z) return to previous menu")

        #check the validity of user input
        inputCheck = 1

        while inputCheck == 1:
            #user input
            ActionChoice = input(": ")

            if ActionChoice == "A" or ActionChoice == "a":

                errorCheck = SpellChecker.SpellChecker(FileType, MethodType, 0)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "B" or ActionChoice == "b":

                errorCheck = SpellChecker.SpellChecker(FileType, MethodType, 1)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "C" or ActionChoice == "c":

                errorCheck = SpellChecker.SpellChecker(FileType, MethodType, 2)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "D" or ActionChoice == "d":

                errorCheck = SpellChecker.SpellChecker(FileType, MethodType, 3)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "E" or ActionChoice == "e":

                errorCheck = SpellChecker.SpellChecker(FileType, MethodType, 4)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "F" or ActionChoice == "f":

                errorCheck = SpellChecker.SpellChecker(FileType, MethodType, 5)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "G" or ActionChoice == "g":

                errorCheck = SpellChecker.SpellChecker(FileType, MethodType, 6)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "H" or ActionChoice == "h":

                errorCheck = SpellChecker.SpellChecker(FileType, MethodType, 7)

                #if -1, an error occured
                #if 0, then reload the page
                if errorCheck == 0: inputCheck = 2
                elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "Z" or ActionChoice == "z":
                inputCheck = 0

            else: print("Invalid response, please try again")

        # if inputCheck == 0 that means that the user wishes to leave the system
        # otherwise it means that the user still wishes to do other things with the system or and error has occured (system will loop)
        if inputCheck == 0: returnCheck = 0
        elif inputCheck == -1: returnCheck = -1
    
    return returnCheck

def AutoTesterMenu():
    #check to see if the user wants to return to the main menu
    returnCheck = 1

    #will loop till the user exits via typing z or Z on the keyboard in the main menu
    while returnCheck == 1:
        #clear the screen so that only relevant system text is shown
        os.system("cls")

        #show user options for the corpus
        print("options for automated testing (indicate which option by typing its asscociated letter):")
        print("(A) Full comparison of all avaliable methods and algorithms on wordlist with no duplicates")
        print("(B) Full comparison of all avaliable methods and algorithms on wordlist with no duplicates sorted alphabetically (merge sort)")
        print("(Z) return to main menu\n")

        inputCheck = 1

        while inputCheck == 1:
            #user input
            ActionChoice = input(": ")
                
            if ActionChoice == "A" or ActionChoice == "a":

                #check if the file exists already
                if os.path.exists("./GeneratedFiles/LWND.txt") == True:
                    print("A file of this type exists, do you want to use it (y/n)?")
                    ActionChoice = input(": ")

                    if ActionChoice == "Y" or ActionChoice == "y":
                        #perform the automated spell checking
                        errorCheck = SpellChecker.AutoSpellChecker(1)

                        if errorCheck == 0: inputCheck = 2
                        elif errorCheck == -1: inputCheck = -1
                    else:
                        errorCheck = ListGenerator.DataReader(0, 0, 1)
                        #if -1, an error occured.
                        #if 0, then reload the page again
                        if errorCheck == 0:
                            #perform the automated spell checking
                            errorCheck = SpellChecker.AutoSpellChecker(1)

                            if errorCheck == 0: inputCheck = 2
                            elif errorCheck == -1: inputCheck = -1

                        elif errorCheck == -1: inputCheck = -1
                else:
                    errorCheck = ListGenerator.DataReader(0, 0, 1)

                    #if -1, an error occured.
                    #if 0, then reload the page again
                    if errorCheck == 0:
                        #perform the automated spell checking
                        errorCheck = SpellChecker.AutoSpellChecker(1)

                        if errorCheck == 0: inputCheck = 2
                        elif errorCheck == -1: inputCheck = -1

                    elif errorCheck == -1: inputCheck = -1

            if ActionChoice == "B" or ActionChoice == "b":
                
                #check if the file exists already
                if os.path.exists("./GeneratedFiles/LWNDS.txt") == True:
                    print("A file of this type exists, do you want to use it (y/n)?")
                    ActionChoice = input(": ")

                    if ActionChoice == "Y" or ActionChoice == "y":
                        #perform the automated spell checking
                        errorCheck = SpellChecker.AutoSpellChecker(3)

                        if errorCheck == 0: inputCheck = 2
                        elif errorCheck == -1: inputCheck = -1
                    else:

                        errorCheck = ListGenerator.DataReader(0, 1, 1)
                        #if -1, an error occured.
                        #if 0, then reload the page again
                        if errorCheck == 0:
                            #perform the automated spell checking
                            errorCheck = SpellChecker.AutoSpellChecker(3)

                            if errorCheck == 0: inputCheck = 2
                            elif errorCheck == -1: inputCheck = -1

                        elif errorCheck == -1: inputCheck = -1
                else:
                    errorCheck = ListGenerator.DataReader(0, 1, 1)

                    #if -1, an error occured.
                    #if 0, then reload the page again
                    if errorCheck == 0:
                        #perform the automated spell checking
                        errorCheck = SpellChecker.AutoSpellChecker(3)

                        if errorCheck == 0: inputCheck = 2
                        elif errorCheck == -1: inputCheck = -1

                    elif errorCheck == -1: inputCheck = -1

            elif ActionChoice == "Z" or ActionChoice ==  "z": inputCheck = 0
            
            #if the user types an invalid answer
            else: print("Invalid response, please try again")

        # if inputCheck == 0 that means that the user wishes to leave the system
        # otherwise it means that the user still wishes to do other things with the system or and error has occured (system will loop)
        if inputCheck == 0: returnCheck = 0
        elif inputCheck == -1: returnCheck = -1

    return returnCheck

if __name__ == "__main__":
    
    #go to the main menu to decide what functions to perform
    mainMenu()