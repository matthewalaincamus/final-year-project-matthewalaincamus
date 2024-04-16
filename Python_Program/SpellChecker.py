import os

#main menu for spell checker
def SpellChecker(FileType : int, CheckType : int, AlgorithmCheck : int):
    
    #check to see if the user wants to return to the main menu
    returnCheck = 1

    while returnCheck == 1:
        #declutter the screen
        os.system("cls")

        if AlgorithmCheck == 0: print("Simple Check Spell Check:")
        elif AlgorithmCheck == 1: print("Simple Check Spell Check with Levenshtein Distance based suggestions:")
        elif AlgorithmCheck == 2: print("Simple Check Spell Check with Hamming Distance based suggestions:")
        elif AlgorithmCheck == 3: print("Simple Check Spell Check with Sorensen-Dice Coefficient based suggestions:")
        elif AlgorithmCheck == 4: print("Simple Check Spell Check with Optimal String Alignment Distance based suggestions:")
        elif AlgorithmCheck == 5: print("Simple Check Spell Check with Damereau-Levenshtein Distance based suggestions:")
        elif AlgorithmCheck == 6: print("Simple Check Spell Check with Jaro Similarity based suggestions:")
        elif AlgorithmCheck == 7: print("Simple Check Spell Check with Jaro-Winkler Similarity based suggestions:")
        else:
            print("Error: LevCheck value wrong")
            return -1
        
        print("**Current Filetype selected: ")

        #display file type info
        if FileType == 0: print("*text with duplicate")
        elif FileType == 1: print("*text without duplicates")
        elif FileType== 2: print("*text with duplicates sorted in alphabetical order")
        elif FileType == 3: print("*text without duplicates sorted in alphabetical order")

        #display the method used
        if CheckType == 0: print("*Method: Word list with linear search")
        elif CheckType == 1: print("*Method: Word list with binary search")
        elif CheckType == 2: print("*Method: Word list as a hash table")
        elif CheckType == 3: print("*Method(s): All")

        print("")

        print("Please enter the word you wish to spell check")
        print("RULES:")
        print("- no numbers or non alphabet characters")
        print("- maximum length string of 25 characters")
        print("- all characters must be in lowercase")
        print("If you wish to return to the previous screen, please type '?' and then press enter")

        #check the validity of the user input
        inputCheck = 1

        while inputCheck == 1:
            #user input
            ActionChoice = input(": ")

            #exit clause
            if ActionChoice == "?": return 0

            #need to check if a given word matches the criteria
            WordCheck = 0

            #check letters for non-alpha characters
            for letter in ActionChoice:
                if ord(letter) < 97 or ord(letter) > 122: WordCheck+=1

            #check that no errors occured, and that the length of the word is valid
            if WordCheck > 0 or len(ActionChoice) > 25:
                print("Please enter a valid word")
            else: inputCheck = 0

        if (CheckType == 0 or CheckType == 1):
            WordArray = WordFetcher(FileType)


    return 0

#perform linear search method
def linearCheck(WordArray, WordCount : int, wordString : str, AlgorithmCheck : int, TestFlag : bool ):
    return 0

#perform binary search method
def binaryCheck(WordArray, WordCount : int, wordString : str, AlgorithmCheck : int, TestFlag : bool ):
    return 0

#perform binary search algorithm (recursive)
def binarySearch(WordArray, leftValue : int, rightValue : int, wordString : str):
    return 0

#perform hash table search method
def hashCheck(HashTable, maxSize : int, WordCount : int, wordString : str, AlgorithmCheck : int, TestFlag : bool ):
    return 0

#get words from file into a list
def WordFetcher(FileType : int):
    return 0

#get words from file into a hash table
def initHash(FileType : int, MaxSize : int):
    return 0

#Levenshtein Distance algorithm
def LevenshteinDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    return 0

#Hamming Distance algorithm
def HammingDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    return 0

#Sørensen-Dice Coefficient algorithm
def SorensenDiceCoefficient(string1 : str, string2 : str, len1 : int, len2 : int):
    return 0

#Optimal String Alignment Distance algorithm
def OptimalStringAlignmentDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    return 0

#Damerau-Levenshtein Distance algorithm
def DamerauLevenshteinDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    return 0

#Jaro Similarity algorithm
def JaroSimilarity(string1 : str, string2 : str, len1 : int, len2 : int):
    return 0

#Jaro-Winkler Similarity algorithm
def JaroWinklerSimilarity(string1 : str, string2 : str, len1 : int, len2 : int):
    return 0

#function to handle full testing and comparison of methods and algorithms
def AutoSpellChecker(FileType : int):
    return 0