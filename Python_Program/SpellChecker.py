import os
from timeit import default_timer
import math
from strsimpy.damerau import Damerau
from strsimpy.optimal_string_alignment import OptimalStringAlignment
from strsimpy.levenshtein import Levenshtein
from strsimpy.jaro_winkler import JaroWinkler
from strsimpy.sorensen_dice import SorensenDice

#hash table node
class Node:
    def __init__(self, key, Word, AlgorithmDistance):
        self.key = key
        self.Word = Word
        self.AlgorithmDistance = AlgorithmDistance
        self.next = None

class HashTable:
    def __init__(self, maxSize):
        self.maxSize = maxSize
        self.table = [None] * maxSize
        self.wordCount = 0

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

        #for binary and linear search methods
        if (CheckType == 0 or CheckType == 1):
            #use the function to get the words into a list
            WordArray = WordFetcher(FileType)
            if WordArray == -1: return -1

            #linear search
            if CheckType == 0: linearCheck(WordArray, ActionChoice, AlgorithmCheck, 0, "")

            #binary search
            elif CheckType == 1: binaryCheck(WordArray, ActionChoice, AlgorithmCheck, 0, "")

        #for hash table searching
        elif CheckType == 2:
            
            #set maxSize of table to something arbitrary
            MaxSize = 10000

            WordHashTable = initHash(FileType, MaxSize)
            if WordHashTable == -1: return -1

            hashCheck(WordHashTable, ActionChoice, AlgorithmCheck, 0, "")

        #user input to confirm a return to the word menu
        ActionChoice2 = input("Type anything to enter another word: ")

    return 0

#perform linear search method
def linearCheck(WordArray, wordString : str, AlgorithmCheck : int, TestFlag : int, testString : str):
    
    #set the timer
    start = default_timer()

    #values to return in case testFlag == 2
    returnValues = {"SuggestionNumber" : 0 , "TimeTaken" : 0.0}

    #if called from automated testing, print out data differently
    if TestFlag == 1:
        #first print out algorithm used:
        if AlgorithmCheck == 0: print("|%-30s" % ("None"), end='')
        elif AlgorithmCheck == 1: print("|%-30s" % ("LevenshteinDistance"), end='')
        elif AlgorithmCheck == 2: print("|%-30s" % ("HammingDistance"), end='')
        elif AlgorithmCheck == 3: print("|%-30s" % ("SorensenDiceCoefficient"), end='')
        elif AlgorithmCheck == 4: print("|%-30s" % ("OptimalStringAlignmentDistance"), end='')
        elif AlgorithmCheck == 5: print("|%-30s" % ("DamerauLevenshteinDistance"), end='')
        elif AlgorithmCheck == 6: print("|%-30s" % ("JaroSimilarity"), end='')
        elif AlgorithmCheck == 7: print("|%-30s" % ("JaroWinklerSimilarity"), end='')

    #for checking if a word is found
    WordFlag = 0

    #for suggestion calculations
    AlgorithmArray = []

    #automatically approve one letter answers
    if len(wordString) == 1:
        if TestFlag == 0: print("Your word exists in the word list")
        WordFlag+=1

    else:
        #loop through the list linearly until the word is found
        for i in range(len(WordArray)):
            if WordArray[i] == wordString:
                if TestFlag == 0: print("Your word exists in the word list")
                WordFlag+=1
                break
            
            #if an algorithm is active, get the distance between the two words
            if AlgorithmCheck == 1: AlgorithmArray.append(LevenshteinDistance(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
            elif AlgorithmCheck == 2: AlgorithmArray.append(HammingDistance(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
            elif AlgorithmCheck == 3: AlgorithmArray.append(SorensenDiceCoefficient(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
            elif AlgorithmCheck == 4: AlgorithmArray.append(OptimalStringAlignmentDistance(wordString, WordArray[i], len(wordString), len(WordArray[i])))
            elif AlgorithmCheck == 5: AlgorithmArray.append(DamerauLevenshteinDistance(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
            elif AlgorithmCheck == 6: AlgorithmArray.append(JaroSimilarity(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
            elif AlgorithmCheck == 7: AlgorithmArray.append(JaroWinklerSimilarity(wordString, WordArray[i], len(wordString), len(WordArray[i])) )

    
    #if word isn't found, get any suggestions
    if WordFlag == 0:
        if TestFlag == 0: print("Your word doesn't exist in the word list")
        elif TestFlag == 1: print("|%-15s" % ("False"), end='')

        #if anything other than a simple check, get the top 10 suggestions
        if AlgorithmCheck != 0:

            #get top 10 suggestions
            for i in range(10):
                #get smallest distance suggestion and its index
                CurrentMin = min(AlgorithmArray)
                CurrentMinIndex = AlgorithmArray.index(CurrentMin)

                if TestFlag == 0: print("Suggestion %i: %s" % (i+1, WordArray[CurrentMinIndex]))
                elif TestFlag == 1 and i == 0: print("|%-25s|" % (WordArray[CurrentMinIndex]))
                elif TestFlag == 1 and i > 0: print("|%-30s|%-15s|%-25s|" % ("", "",  WordArray[CurrentMinIndex]))
                elif TestFlag == 2 and WordArray[CurrentMinIndex] == testString: returnValues["SuggestionNumber"] = i + 1 

                #update the current min value so its no longer min
                AlgorithmArray[CurrentMinIndex] = 100
        elif AlgorithmCheck == 0 and TestFlag == 1: print("|%-25s|" % ("N/A"))
    elif WordFlag > 0 and TestFlag == 1: print("|%-15s|%-25s|" % ("True", ""))

    #get the time of completion
    TimeTaken = (default_timer() - start) * 1000

    if TestFlag == 0: print("Operation took: %f miliseconds to complete\n" % (TimeTaken))
    elif TestFlag == 1: print("|Time to complete: %-10f ms%-41s|" %  (TimeTaken, ""))
    elif TestFlag == 2: 
        returnValues["TimeTaken"] = TimeTaken
        return returnValues
    return 0

#perform binary search method
def binaryCheck(WordArray, wordString : str, AlgorithmCheck : int, TestFlag : int, testString : str):
    
    #set the timer
    start = default_timer()

    #values to return in case testFlag == 2
    returnValues = {"SuggestionNumber" : 0 , "TimeTaken" : 0.0}

    #if called from automated testing, print out data differently
    if TestFlag == 1:
        #first print out algorithm used:
        if AlgorithmCheck == 0: print("|%-30s" % ("None"), end='')
        elif AlgorithmCheck == 1: print("|%-30s" % ("LevenshteinDistance"), end='')
        elif AlgorithmCheck == 2: print("|%-30s" % ("HammingDistance"), end='')
        elif AlgorithmCheck == 3: print("|%-30s" % ("SorensenDiceCoefficient"), end='')
        elif AlgorithmCheck == 4: print("|%-30s" % ("OptimalStringAlignmentDistance"), end='')
        elif AlgorithmCheck == 5: print("|%-30s" % ("DamerauLevenshteinDistance"), end='')
        elif AlgorithmCheck == 6: print("|%-30s" % ("JaroSimilarity"), end='')
        elif AlgorithmCheck == 7: print("|%-30s" % ("JaroWinklerSimilarity"), end='')

    binaryChecker = binarySearch(WordArray, 0, len(WordArray) - 1, wordString)
    if binaryChecker == -1:
        if TestFlag == 0: print("Your word doesn't exist in the word list")
        elif TestFlag == 1: print("|%-15s" % ("False"), end='')

        if AlgorithmCheck != 0:
            
            #for storing suggestion calculations
            AlgorithmArray = []

            for i in range(len(WordArray)):
                #if an algorithm is active, get the distance between the two words
                if AlgorithmCheck == 1: AlgorithmArray.append(LevenshteinDistance(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
                elif AlgorithmCheck == 2: AlgorithmArray.append(HammingDistance(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
                elif AlgorithmCheck == 3: AlgorithmArray.append(SorensenDiceCoefficient(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
                elif AlgorithmCheck == 4: AlgorithmArray.append(OptimalStringAlignmentDistance(wordString, WordArray[i], len(wordString), len(WordArray[i])))
                elif AlgorithmCheck == 5: AlgorithmArray.append(DamerauLevenshteinDistance(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
                elif AlgorithmCheck == 6: AlgorithmArray.append(JaroSimilarity(wordString, WordArray[i], len(wordString), len(WordArray[i])) )
                elif AlgorithmCheck == 7: AlgorithmArray.append(JaroWinklerSimilarity(wordString, WordArray[i], len(wordString), len(WordArray[i])) )

            #if anything other than a simple check, get the top 10 suggestions
            if AlgorithmCheck != 0:

                #get top 10 suggestions
                for i in range(10):
                    #get smallest distance suggestion and its index
                    CurrentMin = min(AlgorithmArray)
                    CurrentMinIndex = AlgorithmArray.index(CurrentMin)

                    if TestFlag == 0: print("Suggestion %i: %s" % (i+1, WordArray[CurrentMinIndex]))
                    elif TestFlag == 1 and i == 0: print("|%-25s|" % (WordArray[CurrentMinIndex]))
                    elif TestFlag == 1 and i > 0: print("|%-30s|%-15s|%-25s|" % ("", "",  WordArray[CurrentMinIndex]))
                    elif TestFlag == 2 and WordArray[CurrentMinIndex] == testString: returnValues["SuggestionNumber"] = i + 1 

                    #update the current min value so its no longer min
                    AlgorithmArray[CurrentMinIndex] = 100

        elif AlgorithmCheck == 0 and TestFlag == 1: print("|%-25s|" % ("N/A"))

    else:
        if TestFlag == 0: print("Your word does exist in the word list")
        elif TestFlag == 1: print("|%-15s|%-25s|" % ("True", ""))

    #get the time of completion
    TimeTaken = (default_timer() - start) * 1000

    if TestFlag == 0: print("Operation took: %f miliseconds to complete\n" % (TimeTaken))
    elif TestFlag == 1: print("|Time to complete: %-10f ms%-41s|" %  (TimeTaken, ""))
    elif TestFlag == 2: 
        returnValues["TimeTaken"] = TimeTaken
        return returnValues

    return 0

#perform binary search algorithm (iterative)
def binarySearch(WordArray, leftValue : int, rightValue : int, wordString : str):

    while leftValue <= rightValue:

        middleValue = leftValue + (rightValue - leftValue) // 2

        if WordArray[middleValue] > wordString : rightValue = middleValue - 1
        elif WordArray[middleValue] < wordString: leftValue = middleValue + 1
        else: return middleValue

    #if not found, return -1
    return -1

#perform hash table search method
def hashCheck(HashTable : HashTable, wordString : str, AlgorithmCheck : int, TestFlag : int, testString : str):
    
    #set the timer
    start = default_timer()

    #values to return in case testFlag == 2
    returnValues = {"SuggestionNumber" : 0 , "TimeTaken" : 0.0}

    #if called from automated testing, print out data differently
    if TestFlag == 1:
        #first print out algorithm used:
        if AlgorithmCheck == 0: print("|%-30s" % ("None"), end='')
        elif AlgorithmCheck == 1: print("|%-30s" % ("LevenshteinDistance"), end='')
        elif AlgorithmCheck == 2: print("|%-30s" % ("HammingDistance"), end='')
        elif AlgorithmCheck == 3: print("|%-30s" % ("SorensenDiceCoefficient"), end='')
        elif AlgorithmCheck == 4: print("|%-30s" % ("OptimalStringAlignmentDistance"), end='')
        elif AlgorithmCheck == 5: print("|%-30s" % ("DamerauLevenshteinDistance"), end='')
        elif AlgorithmCheck == 6: print("|%-30s" % ("JaroSimilarity"), end='')
        elif AlgorithmCheck == 7: print("|%-30s" % ("JaroWinklerSimilarity"), end='')
    
    #for checking if a word is found
    WordFlag = 0

    #automatically approve one letter answers
    if len(wordString) == 1:
        if TestFlag == 0: print("Your word exists in the word list")
        WordFlag+=1
    else:
        #get the hash value of the word string
        StringHash = 1
        for letter in wordString:
            StringHash = (StringHash * ord(letter)) % HashTable.maxSize
            if StringHash == 0: StringHash += 1

        #get the data from the table using the hash value
        currentNode : Node = HashTable.table[StringHash]

        #check if there is a node matching the user word
        if currentNode is not None:
            if currentNode.Word == wordString:
                if TestFlag == 0: print("Your word exists in the word list")
                WordFlag+=1

            else:
                while currentNode.next is not None:
                    currentNode = currentNode.next
                    if currentNode.Word == wordString:
                        if TestFlag == 0: print("Your word exists in the word list")
                        WordFlag+=1
                        break

    if WordFlag == 0:
        if TestFlag == 0: print("Your word doesn't exist in the word list")
        elif TestFlag == 1: print("|%-15s" % ("False"), end='')

        #if suggestions are needed, loop linearly through the hash table and update
        #the algorithm distance for each word based on the distance between a node and the user word
        if AlgorithmCheck != 0:
            for i in range (HashTable.maxSize):
                if HashTable.table[i] is not None:
                    currentNode = HashTable.table[i]

                    #if there is only one node in a linked list
                    if currentNode.next is None:
                        #if an algorithm is active, get the distance between the two words
                        if AlgorithmCheck == 1: currentNode.AlgorithmDistance = LevenshteinDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 2: currentNode.AlgorithmDistance = HammingDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 3: currentNode.AlgorithmDistance = SorensenDiceCoefficient(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 4: currentNode.AlgorithmDistance = OptimalStringAlignmentDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 5: currentNode.AlgorithmDistance = DamerauLevenshteinDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 6: currentNode.AlgorithmDistance = JaroSimilarity(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 7: currentNode.AlgorithmDistance = JaroWinklerSimilarity(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
    
                    #if there is more than one node
                    else:
                        #check the first node
                        if AlgorithmCheck == 1: currentNode.AlgorithmDistance = LevenshteinDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 2: currentNode.AlgorithmDistance = HammingDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 3: currentNode.AlgorithmDistance = SorensenDiceCoefficient(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 4: currentNode.AlgorithmDistance = OptimalStringAlignmentDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 5: currentNode.AlgorithmDistance = DamerauLevenshteinDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 6: currentNode.AlgorithmDistance = JaroSimilarity(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 7: currentNode.AlgorithmDistance = JaroWinklerSimilarity(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        
                        while currentNode.next is not None:
                            currentNode = currentNode.next

                            #if an algorithm is active, get the distance between the two words
                            if AlgorithmCheck == 1: currentNode.AlgorithmDistance = LevenshteinDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                            elif AlgorithmCheck == 2: currentNode.AlgorithmDistance = HammingDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                            elif AlgorithmCheck == 3: currentNode.AlgorithmDistance = SorensenDiceCoefficient(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                            elif AlgorithmCheck == 4: currentNode.AlgorithmDistance = OptimalStringAlignmentDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                            elif AlgorithmCheck == 5: currentNode.AlgorithmDistance = DamerauLevenshteinDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                            elif AlgorithmCheck == 6: currentNode.AlgorithmDistance = JaroSimilarity(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                            elif AlgorithmCheck == 7: currentNode.AlgorithmDistance = JaroWinklerSimilarity(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
    
            #get the top 10 suggestions for the user word
            for i in range(10):
                currentMin = 100
                currentMinIndex = 0
                currentMinKeyIndex = 0

                for j in range (HashTable.maxSize):
                    if HashTable.table[j] is not None:
                        currentNode = HashTable.table[j]

                        #if there is only one node in a linked list
                        if currentNode.next is None:
                            if currentNode.AlgorithmDistance < currentMin:
                                currentMin = currentNode.AlgorithmDistance
                                currentMinIndex = j
                                currentMinKeyIndex = currentNode.key
        
                        #if there is more than one node
                        else:
                            #check the first node in the linked list
                            if currentNode.AlgorithmDistance < currentMin:
                                    currentMin = currentNode.AlgorithmDistance
                                    currentMinIndex = j
                                    currentMinKeyIndex = currentNode.key

                            #then the rest
                            while currentNode.next is not None:                            
                                currentNode = currentNode.next
                                
                                if currentNode.AlgorithmDistance < currentMin:
                                    currentMin = currentNode.AlgorithmDistance
                                    currentMinIndex = j
                                    currentMinKeyIndex = currentNode.key
                            
                
                #update the min node to be large
                updateNode = HashTable.table[currentMinIndex]
                for k in range(currentMinKeyIndex):
                    updateNode = updateNode.next

                updateNode.AlgorithmDistance = 100

                if TestFlag == 0: print("Suggestion %i: %s" % (i+1, updateNode.Word))
                elif TestFlag == 1 and i == 0: print("|%-25s|" % (updateNode.Word))
                elif TestFlag == 1 and i > 0: print("|%-30s|%-15s|%-25s|" % ("", "",  updateNode.Word))
                elif TestFlag == 2 and updateNode.Word == testString: returnValues["SuggestionNumber"] = i + 1 
        
        elif AlgorithmCheck == 0 and TestFlag == 1: print("|%-25s|" % ("N/A"))
    
    elif WordFlag > 0 and TestFlag == 1: print("|%-15s|%-25s|" % ("True", ""))


    #get the time of completion
    TimeTaken = (default_timer() - start) * 1000

    if TestFlag == 0: print("Operation took: %f miliseconds to complete\n" % (TimeTaken))
    elif TestFlag == 1: print("|Time to complete: %-10f ms%-41s|" %  (TimeTaken, ""))
    elif TestFlag == 2: 
        returnValues["TimeTaken"] = TimeTaken
        return returnValues

    return 0

#get words from file into a list
def WordFetcher(FileType : int):

    #list structure
    Wordlist = []

    #open required wordfile
    try:
        if FileType == 0:
            fp = open("./GeneratedFiles/LWD.txt", "r")
        elif FileType == 1:
            fp = open("./GeneratedFiles/LWND.txt", "r")
        elif FileType == 2:
            fp = open("./GeneratedFiles/LWDS.txt", "r")
        elif FileType == 3:
            fp = open("./GeneratedFiles/LWNDS.txt", "r")
    except FileNotFoundError:
        print("Error: unable to open file")
        return -1
    
    Lines = fp.readlines()

    #close after use
    fp.close()

    for word in Lines:
        Wordlist.append(word[:-1])

    return Wordlist

#get words from file into a hash table
def initHash(FileType : int, MaxSize : int):
    
    #list structure
    WordHashTable = HashTable(MaxSize)

    #open required wordfile
    try:
        if FileType == 0:
            fp = open("./GeneratedFiles/LWD.txt", "r")
        elif FileType == 1:
            fp = open("./GeneratedFiles/LWND.txt", "r")
        elif FileType == 2:
            fp = open("./GeneratedFiles/LWDS.txt", "r")
        elif FileType == 3:
            fp = open("./GeneratedFiles/LWNDS.txt", "r")
    except FileNotFoundError:
        print("Error: unable to open file")
        return -1
    
    #get all data from the file
    Lines = fp.readlines()

    #close after use
    fp.close()

    #get all words in the word list
    for word in Lines:
        #get the hash value for the word
        hashValue = 1
        for letter in word[:-1]:
            hashValue = (hashValue * ord(letter)) % MaxSize
            #so most of the nodes don't congregate at zero
            if hashValue == 0: hashValue += 1

        #if there is no item at the given hash index, make a new node there
        #otherwise, append it the the linked list
        if WordHashTable.table[hashValue] is None:
            WordHashTable.table[hashValue] = Node(0, word[:-1], 100)

        else:
            currentNode = WordHashTable.table[hashValue]
            while currentNode:
                if currentNode.next == None:
                    currentNode.next = Node(currentNode.key + 1, word[:-1], 100)
                    break
                else:
                    currentNode = currentNode.next

        #update the word count of the hash table
        WordHashTable.wordCount += 1

    return WordHashTable

#Levenshtein Distance algorithm
def LevenshteinDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    levenshtein = Levenshtein()
    return levenshtein.distance(string1, string2)
    '''
    #array to store results of calculations
    CalculationArray = [[0 for x in range(len2 + 1)] for y in range(len1+1)]

    #add starting values to calculation array
    for i in range(len1+1): CalculationArray[i][0] = i
    for j in range(len2+1): CalculationArray[0][j] = j

    #fill in the matrix using dynamic programming
    for i in range(1, len1+1):
        for j in range(1, len2+1):
            if string1[i - 1] == string2[j - 1]:
                #characters match, no operation needed
                CalculationArray[i][j] = CalculationArray[i-1][j-1]
            else:
                #characters don't match, choose minimum cost among insertion, deletion, substitution
                CalculationArray[i][j] = 1 + min(
                                        CalculationArray[i][j-1],
                                        CalculationArray[i-1][j],
                                        CalculationArray[i-1][j-1])
    return CalculationArray[len1][len2]
    '''

#Hamming Distance algorithm
def HammingDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    
    #strings need to be same length
    if len1 != len2: return 100; 

    #score variable
    HammingScore = 0

    #if a char is different, add 1 to the score
    for i in range(len1):
        if string1[i] != string2[i]: HammingScore+=1

    return HammingScore

#Sørensen-Dice Coefficient algorithm
def SorensenDiceCoefficient(string1 : str, string2 : str, len1 : int, len2 : int):
    SD = SorensenDice()
    return SD.distance(string1, string2)
    '''
    #number of bigrams that match
    matches = 0.0

    i = 0
    j = 0

    #loop through, getting the number of bigrams that match and updating the count
    while i < len1 - 1 and j < len2 - 1:
        bigram1 = "" + string1[i] + string2[i+1]
        bigram2 = "" + string2[j] + string2[j+1]

        if bigram1 == bigram2: matches+=1

        i+=1
        j+=1

    #would usually be a value between 0 and 1, but for uniformity of keeping all algorithm values
    # int, times the value by 100 and use that instead
    #also, minus the value from 100, so that similar values are close to 0 than 100 so the 
    # top 10 results calculator and just look for the smallest values
    return 100 - (100 * ( (2 * matches) / ( (len1 - 1) + (len2 - 1) ) ) )
    '''

#Optimal String Alignment Distance algorithm
def OptimalStringAlignmentDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    OSA = OptimalStringAlignment()
    return OSA.distance(string1, string2)
    '''
    #array to store results of calculations
    CalculationArray = [ [0 for x in range(len2+1)] for y in range (len1+1)]

    #add starting values to calculation array
    for i in range(len1 + 1): CalculationArray[i][0] = i
    for j in range(len2 + 1): CalculationArray[0][j] = j

    for i in range(len1):
        for j in range(len2):
            cost = 0

            if string1[i] == string2[j]: cost = 0
            else: cost = 1

            CalculationArray[i+1][j+1] = min(CalculationArray[i][j+1] + 1, #deletion
                                            CalculationArray[i+1][j] + 1, #insertion
                                            CalculationArray[i][j] + cost) #substitution
            
            if i > 0 and j > 0 and string1[i] == string2[j-1] and string1[i-1] == string2[j]:
                CalculationArray[i][j] = min(CalculationArray[i][j],
                                                CalculationArray[i-1][j-1] + 1) #transposition

    return CalculationArray[len1][len2]
    '''

#Damerau-Levenshtein Distance algorithm
def DamerauLevenshteinDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    damerau = Damerau()
    return damerau.distance(string1, string2)
    '''
    # Used to prevent transpositions for first characters
    MaxDistance = len1 + len2

    #2D array for storing the results of calculations
    CalculationMatrix  =  [ [0 for x in range(len2+2)] for y in range (len1+2)]

    CalculationMatrix[0][0] = MaxDistance
    for i in range(len1+1):
        CalculationMatrix[i+1][1] = i
        CalculationMatrix[i+1][0] = MaxDistance
    for j in range(len2):
        CalculationMatrix[1][j+1] = j
        CalculationMatrix[0][j+1] = MaxDistance

    # Holds last row each element was encountered
    last_row = {}

    #calculation loop
    for i in range(1, len1 + 1):
        #current string 1 character
        string1Char = string1[i-1]

        # Column of last match on this row
        lastMatchingCol = 0

        for j in range(1, len2 + 1):
            # Current string 2 character
            string2Char = string2[j-1]

            # Last row with matching character
            lastMatchingRow = last_row.get(string2Char, 0)

            # Cost of substitution
            cost = 0 if string1Char == string2Char else 1

            # Compute smallest distance
            CalculationMatrix[i+1][j+1] = min(
                CalculationMatrix[i][j] + cost, # Substitution
                CalculationMatrix[i+1][j] + 1,  # Addition
                CalculationMatrix[i][j+1] + 1,  # Deletion
                CalculationMatrix[lastMatchingRow][lastMatchingCol]
                    + (i - lastMatchingRow - 1) + 1
                    + (j - lastMatchingCol - 1)) # Transposition

            # If there was a match, update last_match_col
            if cost == 0:
                lastMatchingCol = j

        # Update last row for current character
        last_row[string1Char] = i

    # Return last element
    return CalculationMatrix[-1][-1]
    '''

#Jaro Similarity algorithm
def JaroSimilarity(string1 : str, string2 : str, len1 : int, len2 : int):
    
    #max distance for which matching is allowed
    maxDist = math.floor(max(len1, len2) / 2) - 1

    #count for number of total matches
    matches = 0

    #hashes to store matches
    String1Matches = [0 for x in range(len1)]
    String2Matches = [0 for y in range(len2)]

    for i in range(len1): String1Matches[i] = 0
    for j in range(len2): String2Matches[j] = 0

    #traverse the first string
    for i in range(len1):
        #check if there are any matches
        for j in range(max(0, i - maxDist), min(len2, i + maxDist + 1)):
            if string1[i] == string2[j] and String2Matches[j] == 0:
                String1Matches[i] = 1
                String2Matches[j] = 1
                matches+=1
                break

    #if nothing matches, return the larges value possible
    if matches == 0: return 100

    #number of transpositions
    t = 0
    point = 0

    for i in range(len1):
        if String1Matches[i]:
            while String2Matches[point] == 0: point+=1

            if string1[i] != string2[point]: t+=1

            point+=1
    
    t = t // 2

    # Jaro Similarity Returned
    #times by 100 and then subtract from 100 so that similar values are converted to int and close to zero
    return 100 - 100 * (matches  / len1  + matches / len2  + (matches - t) / matches) / 3.0

#Jaro-Winkler Similarity algorithm
def JaroWinklerSimilarity(string1 : str, string2 : str, len1 : int, len2 : int):

    jaroWinkler = JaroWinkler()
    return jaroWinkler.distance(string1, string2)

    '''
    #get the original jaro distance
    jaroDistance = (100 - JaroSimilarity(string1, string2, len1, len2)) / 100

    #if the similarity is above a threshold
    if jaroDistance > 0.7:
        Prefix = 0

        for i in range(0, min(len1, len2)):
            #if the characters match
            if string1[i] == string2[i]: Prefix+=1
            else: break

        #maximum of 4 characters are allowed in prefix
        Prefix = min(4, Prefix)

        #calculate jaro winkler Similarity
        jaroDistance += 0.1 * Prefix * (1 - jaroDistance)

    #return jaro distance, making sure values are int and similar values are close to zero
    return 100 - (100 * jaroDistance)
    '''

#function to handle full testing and comparison of methods and algorithms
def AutoSpellChecker(FileType : int):
    #check to see if the user wants to return to the main menu
    returnCheck = 1

    #will loop till the user exits via typing z or Z on the keyboard in the main menu
    while returnCheck == 1:
        os.system("cls")

        print("Full Spell Check:")
        
        print("**Current Filetype selected: ")

        #display file type info
        if FileType == 0: print("*text with duplicate")
        elif FileType == 1: print("*text without duplicates")
        elif FileType== 2: print("*text with duplicates sorted in alphabetical order")
        elif FileType == 3: print("*text without duplicates sorted in alphabetical order")

        print("")

        print("Please enter the word you wish to spell check")
        print("RULES:")
        print("- no numbers or non alphabet characters")
        print("- maximum length string of 25 characters")
        print("- all characters must be in lowercase\n")
        print("If you wish to return to the previous screen, please type '?' and then press enter")
        print("If you wish to launch the mispelled word tester, please type '!'")

        inputCheck = 1

        while inputCheck == 1:
            #user input
            ActionChoice = input(": ")

            #question mark denotes return to menu
            if ActionChoice ==  "?": return 0
            elif ActionChoice == "!": break

            WordCheck = 0

            #check letters for non-alpha characters
            for letter in ActionChoice:
                if ord(letter) < 97 or ord(letter) > 122: WordCheck+=1

            #check that no errors occured, and that the length of the word is valid
            if WordCheck > 0 or len(ActionChoice) > 25:
                print("Please enter a valid word")
            else: inputCheck = 0

        if ActionChoice == "!":
            AlgorithmAssessor(FileType)

        else:
            #start the process of testing every method and algorithm all together

            #first generate the wordlist data structure for linear and binary searching
            WordArray = WordFetcher(FileType)
            if WordArray == -1: return -1

            #then begin testing:

            #**linear:
            print("**Method: Linear Search**")
            print("--------------------------------------------------------------------------")
            print("|Algorithm:                    |StringFound:   |Suggestions:             |")
            print("|------------------------------+---------------+-------------------------|")

            #no algorithm
            linearCheck(WordArray, ActionChoice, 0, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Levenshtein Distance
            linearCheck(WordArray, ActionChoice, 1, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Hamming Distance
            linearCheck(WordArray, ActionChoice, 2, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Sørensen–Dice Coefficient
            linearCheck(WordArray, ActionChoice, 3, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Optimal String Alignment Distance
            linearCheck(WordArray, ActionChoice, 4, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Damerau-Levenshtein Distance
            linearCheck(WordArray, ActionChoice, 5, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Jaro Distance
            linearCheck(WordArray, ActionChoice, 6, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Jaro-Winkler Distance
            linearCheck(WordArray, ActionChoice, 6, 1, "")
            print("--------------------------------------------------------------------------")
            print("\n")

            #**binary:
            print("**Method: Binary Search**")

            #need a sorted array to use this
            if FileType == 3:
                print("--------------------------------------------------------------------------")
                print("|Algorithm:                    |StringFound:   |Suggestions:             |")
                print("|------------------------------+---------------+-------------------------|")

                #no algorithm
                binaryCheck(WordArray, ActionChoice, 0, 1, "")
                print("|------------------------------+---------------+-------------------------|")

                #Levenshtein Distance
                binaryCheck(WordArray, ActionChoice, 1, 1, "")
                print("|------------------------------+---------------+-------------------------|")

                #Hamming Distance
                binaryCheck(WordArray, ActionChoice, 2, 1, "")
                print("|------------------------------+---------------+-------------------------|")

                #Sørensen–Dice Coefficient
                binaryCheck(WordArray, ActionChoice, 3, 1, "")
                print("|------------------------------+---------------+-------------------------|")

                #Optimal String Alignment Distance
                binaryCheck(WordArray, ActionChoice, 4, 1, "")
                print("|------------------------------+---------------+-------------------------|")

                #Damerau-Levenshtein Distance
                binaryCheck(WordArray, ActionChoice, 5, 1, "")
                print("|------------------------------+---------------+-------------------------|")

                #Jaro Distance
                binaryCheck(WordArray, ActionChoice, 6, 1, "")
                print("|------------------------------+---------------+-------------------------|")

                #Jaro-Winkler Distance
                binaryCheck(WordArray, ActionChoice, 7, 1, "")
                print("--------------------------------------------------------------------------")

            else: print("Only possible on sorted data")

            print("\n")

            #Hash Table:

            #max size of hash table
            MaxSize = 10000

            #create the hash table from the list of words
            WordHashTable : HashTable = initHash(FileType, MaxSize)
            if WordHashTable == -1: return -1

            print("**Method: Hash Table**")
            print("--------------------------------------------------------------------------")
            print("|Algorithm:                    |StringFound:   |Suggestions:             |")
            print("|------------------------------+---------------+-------------------------|")

            #no algorithm
            hashCheck(WordHashTable, ActionChoice, 0, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Levenshtein Distance
            hashCheck(WordHashTable, ActionChoice, 1, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Hamming Distance
            hashCheck(WordHashTable, ActionChoice, 2, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Sørensen–Dice Coefficient
            hashCheck(WordHashTable, ActionChoice, 3, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Optimal String Alignment Distance
            hashCheck(WordHashTable, ActionChoice, 4, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Damerau-Levenshtein Distance
            hashCheck(WordHashTable, ActionChoice, 5, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Jaro Distance
            hashCheck(WordHashTable, ActionChoice, 6, 1, "")
            print("|------------------------------+---------------+-------------------------|")

            #Jaro-Winkler Distance
            hashCheck(WordHashTable, ActionChoice, 7, 1, "")

            print("--------------------------------------------------------------------------")
            print("\n")

        #user input (just to confirm that the user can return back to the previous menu)
        ActionChoice2 = input("Type anything to enter another word: ")

    return 0

def AlgorithmAssessor(FileType : int):
    #word array for storing all the words
    MisspelledWordArray = {}
    DuplicateCheckArray = []
    IncorrectCount = 0
    CorrectCount = 0
    
    #open mispelled word files

    fp = open("../corpus/Words-Misspelled/aspell.dat.txt","r")
    Lines = fp.readlines()

    CurrentMasterWord = ""
    #for checking if a word only has chars from a - z
    ValidCheck = 0
    #for checking if a correct word is valid or not (all its subsequent incorrect spellings will be ignored)
    CorrectWordCheck = 0

    #read all the words into a large dictionary 
    for line in Lines:
        #correctly spelt words
        if line[0] == '$':
            CorrectWordCheck = 0
            ValidCheck=0
            for letter in line[1:-1]:
                #check all the characters are valid
                if ord(letter) < 97 or ord(letter) > 122:
                    ValidCheck+=1
            if ValidCheck == 0:
                #check for duplicates, if not then add
                if line[1:-1] not in MisspelledWordArray.keys():
                    #allow its children to be validated
                    CorrectWordCheck = 0

                    MisspelledWordArray[line[1:-1]] = []
                    CurrentMasterWord = line[1:-1]
                    CorrectCount+=1

                else:
                    #prevent its children from being validated
                    CorrectWordCheck = 1
            else: 
                #prevent its children from being validated
                CorrectWordCheck = 1

        #incorrectly spelt words
        else:
            for letter in line[:-1]:
                #check each letter is valid
                if ord(letter) < 97 or ord(letter) > 122:
                    ValidCheck+=1
            
            if ValidCheck == 0 and CorrectWordCheck == 0:
                if line[:-1] not in DuplicateCheckArray:
                    MisspelledWordArray[CurrentMasterWord].append(line[:-1])
                    DuplicateCheckArray.append((line[:-1]))
                    IncorrectCount+=1
    #close the file each time
    fp.close()

    #print(MisspelledWordArray)
    
    #begin testing loop
    print("Correct Words:" , len(MisspelledWordArray.keys()), ", Incorrect Words:", IncorrectCount)
    
    #make a number of variables for calculating the average time and suggestion number for each algorithm
    #levenshtein distance
    LevenshteinDistanceMissCount = [0, 0, 0]

    #Hamming distance
    HammingDistanceMissCount = [0, 0, 0]

    #sorensen-dice coefficient
    SorensenDiceMissCount = [0, 0, 0]

    #optimal string alignment distance
    OptimalStringAlignmentMissCount = [0, 0, 0]

    #damerau-levenshtein distance
    DamerauLevensheinMissCount = [0, 0, 0]

    #Jaro similary
    JaroDistanceMissCount = [0, 0, 0]

    #JaroWinklerSimilarity
    JaroWinklerDistanceMissCount = [0, 0, 0]
    

    #first generate the wordlist data structure for linear and binary searching
    WordArray = WordFetcher(FileType)
    if WordArray == -1: return -1
    
    #max size of hash table
    MaxSize = 10000

    #create the hash table from the list of words
    WordHashTable : HashTable = initHash(FileType, MaxSize)
    if WordHashTable == -1: return -1

    if (FileType == 1): Wfp = open("../AutomatedTestingResults/PythonProgramResultsType0.txt", "w")
    elif (FileType ==3): Wfp = open("../AutomatedTestingResults/PythonProgramResultsType1.txt", "w")

    Wfp.write("CorrectCount: %d, IncorrectCount: %d" % (len(MisspelledWordArray.keys()), IncorrectCount))
    Wfp.write('\n')

    Wfp.write("suggestion numbers and runtimes")
    Wfp.write('\n')
    

    for CorrectWord in MisspelledWordArray.keys():
        for MisspelledWord in MisspelledWordArray[CorrectWord]:

            print("%s -> %s" % (MisspelledWord, CorrectWord))

            #no algorithm
            returnedValues = linearCheck(WordArray, MisspelledWord, 0, 2, CorrectWord)
            
            Wfp.write("L/NO: %lf\n" %(returnedValues["TimeTaken"]))
            
            #Levenshtein Distance
            returnedValues = linearCheck(WordArray,MisspelledWord, 1, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: LevenshteinDistanceMissCount[0]+=1
            
            Wfp.write("L/LD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Hamming Distance
            returnedValues = linearCheck(WordArray, MisspelledWord, 2, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: HammingDistanceMissCount[0]+=1

            Wfp.write("L/HD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Sørensen–Dice Coefficient
            returnedValues = linearCheck(WordArray, MisspelledWord, 3, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: SorensenDiceMissCount[0]+=1

            Wfp.write("L/SD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Optimal String Alignment Distance
            returnedValues = linearCheck(WordArray, MisspelledWord, 4, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: OptimalStringAlignmentMissCount[0]+=1

            Wfp.write("L/OS: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Damerau-Levenshtein Distance
            returnedValues = linearCheck(WordArray, MisspelledWord, 5, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: DamerauLevensheinMissCount[0]+=1

            Wfp.write("L/DL: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Jaro Distance
            returnedValues = linearCheck(WordArray, MisspelledWord, 6, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: JaroDistanceMissCount[0]+=1

            Wfp.write("L/JD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Jaro-Winkler Distance
            returnedValues = linearCheck(WordArray, MisspelledWord, 7, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: JaroWinklerDistanceMissCount[0]+=1

            Wfp.write("L/LW: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #**binary:
            #need a sorted array to use this
            if FileType == 3:
                #no algorithm
                returnedValues = binaryCheck(WordArray, MisspelledWord, 0, 2, CorrectWord)
                
                Wfp.write("B/NO: %lf\n" % (returnedValues["TimeTaken"]))

                #Levenshtein Distance
                returnedValues = binaryCheck(WordArray, MisspelledWord, 1, 2, CorrectWord)
                if returnedValues["SuggestionNumber"] == 0: LevenshteinDistanceMissCount[1]+=1
                
                Wfp.write("B/LD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

                #Hamming Distance
                returnedValues = binaryCheck(WordArray, MisspelledWord, 2, 2, CorrectWord)
                if returnedValues["SuggestionNumber"] == 0: HammingDistanceMissCount[1]+=1

                Wfp.write("B/HD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

                #Sørensen–Dice Coefficient
                returnedValues = binaryCheck(WordArray, MisspelledWord, 3, 2, CorrectWord)
                if returnedValues["SuggestionNumber"] == 0: SorensenDiceMissCount[1]+=1
                
                Wfp.write("B/SD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

                #Optimal String Alignment Distance
                returnedValues = binaryCheck(WordArray, MisspelledWord, 4, 2, CorrectWord)
                if returnedValues["SuggestionNumber"] == 0: OptimalStringAlignmentMissCount[1]+=1

                Wfp.write("B/OS: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

                #Damerau-Levenshtein Distance
                returnedValues = binaryCheck(WordArray, MisspelledWord, 5, 2, CorrectWord)
                if returnedValues["SuggestionNumber"] == 0: DamerauLevensheinMissCount[1]+=1

                Wfp.write("B/DL: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

                #Jaro Distance
                returnedValues = binaryCheck(WordArray, MisspelledWord, 6, 2, CorrectWord)
                if returnedValues["SuggestionNumber"] == 0: JaroDistanceMissCount[1]+=1
                
                Wfp.write("B/JD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

                #Jaro-Winkler Distance
                returnedValues = binaryCheck(WordArray, MisspelledWord, 7, 2, CorrectWord)
                if returnedValues["SuggestionNumber"] == 0: JaroWinklerDistanceMissCount[1]+=1

                Wfp.write("B/JW: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Hashing
            
            #no algorithm
            returnedValues = hashCheck(WordHashTable, MisspelledWord, 0, 2, CorrectWord)

            Wfp.write("H/NO: %lf\n" % (returnedValues["TimeTaken"]))

            #Levenshtein Distance
            returnedValues = hashCheck(WordHashTable, MisspelledWord, 1, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: LevenshteinDistanceMissCount[2]+=1

            Wfp.write("H/LD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Hamming Distance
            returnedValues = hashCheck(WordHashTable, MisspelledWord, 2, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: HammingDistanceMissCount[2]+=1

            Wfp.write("H/HD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Sørensen–Dice Coefficient
            returnedValues = hashCheck(WordHashTable, MisspelledWord, 3, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: SorensenDiceMissCount[2]+=1

            Wfp.write("H/SD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Optimal String Alignment Distance
            returnedValues = hashCheck(WordHashTable, MisspelledWord, 4, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: OptimalStringAlignmentMissCount[2]+=1

            Wfp.write("H/OS: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Damerau-Levenshtein Distance
            returnedValues = hashCheck(WordHashTable, MisspelledWord, 5, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: DamerauLevensheinMissCount[2]+=1

            Wfp.write("H/DL: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Jaro Distance
            returnedValues = hashCheck(WordHashTable, MisspelledWord, 6, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: JaroDistanceMissCount[2]+=1

            Wfp.write("H/JD: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))

            #Jaro-Winkler Distance
            returnedValues = hashCheck(WordHashTable, MisspelledWord, 7, 2, CorrectWord)
            if returnedValues["SuggestionNumber"] == 0: JaroWinklerDistanceMissCount[2]+=1

            Wfp.write("H/JW: %ld : %lf\n" % (returnedValues["SuggestionNumber"], returnedValues["TimeTaken"]))
        
    Wfp.write("Total Miss Counts:\n")
    Wfp.write("LD: L:%d, B:%d, H:%d\n" % (LevenshteinDistanceMissCount[0], LevenshteinDistanceMissCount[1],LevenshteinDistanceMissCount[2]))
    Wfp.write("HD: L:%d, B:%d, H:%d\n" % (HammingDistanceMissCount[0], HammingDistanceMissCount[1],HammingDistanceMissCount[2]))
    Wfp.write("SD: L:%d, B:%d, H:%d\n" % (SorensenDiceMissCount[0], SorensenDiceMissCount[1],SorensenDiceMissCount[2]))
    Wfp.write("OS: L:%d, B:%d, H:%d\n" % (OptimalStringAlignmentMissCount[0], OptimalStringAlignmentMissCount[1],OptimalStringAlignmentMissCount[2]))
    Wfp.write("DL: L:%d, B:%d, H:%d\n" % (DamerauLevensheinMissCount[0], DamerauLevensheinMissCount[1],DamerauLevensheinMissCount[2]))
    Wfp.write("JD: L:%d, B:%d, H:%d\n" % (JaroDistanceMissCount[0], JaroDistanceMissCount[1],JaroDistanceMissCount[2]))
    Wfp.write("JW: L:%d, B:%d, H:%d\n" % (JaroWinklerDistanceMissCount[0], JaroWinklerDistanceMissCount[1],JaroWinklerDistanceMissCount[2]))

    Wfp.close()
    
    FinalAction = input("Type anything to enter another word: ")
    return 0
