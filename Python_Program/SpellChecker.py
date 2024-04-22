import os
from timeit import default_timer
import math

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
            if CheckType == 0: linearCheck(WordArray, ActionChoice, AlgorithmCheck, False)

            #binary search
            elif CheckType == 1: binaryCheck(WordArray, ActionChoice, AlgorithmCheck, False)

        #for hash table searching
        elif CheckType == 2:
            
            #set maxSize of table to something arbitrary
            MaxSize = 10000

            WordHashTable = initHash(FileType, MaxSize)
            if WordHashTable == -1: return -1

            hashCheck(WordHashTable, ActionChoice, AlgorithmCheck, False)

        #user input to confirm a return to the word menu
        ActionChoice2 = input("Type anything to enter another word: ")

    return 0

#perform linear search method
def linearCheck(WordArray, wordString : str, AlgorithmCheck : int, TestFlag : bool ):
    
    #set the timer
    start = default_timer()

    #if called from automated testing, print out data differently
    if TestFlag == True:
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
        if TestFlag == False: print("Your word exists in the word list")
        WordFlag+=1

    else:
        #loop through the list linearly until the word is found
        for i in range(len(WordArray)):
            if WordArray[i] == wordString:
                if TestFlag == False: print("Your word exists in the word list")
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
        if TestFlag == False: print("Your word doesn't exist in the word list")
        elif TestFlag == True: print("|%-15s" % ("False"), end='')

        #if anything other than a simple check, get the top 10 suggestions
        if AlgorithmCheck != 0:

            #get top 10 suggestions
            for i in range(10):
                #get smallest distance suggestion and its index
                CurrentMin = min(AlgorithmArray)
                CurrentMinIndex = AlgorithmArray.index(CurrentMin)

                if TestFlag == False: print("Suggestion %i: %s" % (i+1, WordArray[CurrentMinIndex]))
                elif TestFlag == True and i == 0: print("|%-25s|" % (WordArray[CurrentMinIndex]))
                elif TestFlag == True and i > 0: print("|%-30s|%-15s|%-25s|" % ("", "",  WordArray[CurrentMinIndex]))

                #update the current min value so its no longer min
                AlgorithmArray[CurrentMinIndex] = 100
        elif AlgorithmCheck == 0 and TestFlag == True: print("|%-25s|" % ("N/A"))
    elif WordFlag > 0 and TestFlag == True: print("|%-15s|%-25s|" % ("True", ""))

    #get the time of completion
    TimeTaken = (default_timer() - start) * 1000

    if TestFlag == False: print("Operation took: %f miliseconds to complete\n" % (TimeTaken))
    elif TestFlag == True: print("|Time to complete: %-10f ms%-41s|" %  (TimeTaken, ""))
    return 0

#perform binary search method
def binaryCheck(WordArray, wordString : str, AlgorithmCheck : int, TestFlag : bool):
    
    #set the timer
    start = default_timer()

    #if called from automated testing, print out data differently
    if TestFlag == True:
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
        if TestFlag == False: print("Your word doesn't exist in the word list")
        elif TestFlag == True: print("|%-15s" % ("False"), end='')

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

                    if TestFlag == False: print("Suggestion %i: %s" % (i+1, WordArray[CurrentMinIndex]))
                    elif TestFlag == True and i == 0: print("|%-25s|" % (WordArray[CurrentMinIndex]))
                    elif TestFlag == True and i > 0: print("|%-30s|%-15s|%-25s|" % ("", "",  WordArray[CurrentMinIndex]))

                    #update the current min value so its no longer min
                    AlgorithmArray[CurrentMinIndex] = 100

        elif AlgorithmCheck == 0 and TestFlag == True: print("|%-25s|" % ("N/A"))

    else:
        if TestFlag == False: print("Your word does exist in the word list")
        elif TestFlag == True: print("|%-15s|%-25s|" % ("True", ""))

    #get the time of completion
    TimeTaken = (default_timer() - start) * 1000

    if TestFlag == False: print("Operation took: %f miliseconds to complete\n" % (TimeTaken))
    elif TestFlag == True: print("|Time to complete: %-10f ms%-41s|" %  (TimeTaken, ""))

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
def hashCheck(HashTable : HashTable, wordString : str, AlgorithmCheck : int, TestFlag : bool ):
    
    #set the timer
    start = default_timer()

    #if called from automated testing, print out data differently
    if TestFlag == True:
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
        if TestFlag == False: print("Your word exists in the word list")
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
                if TestFlag == False: print("Your word exists in the word list")
                WordFlag+=1

            else:
                while currentNode.next is not None:
                    currentNode = currentNode.next
                    if currentNode.Word == wordString:
                        if TestFlag == False: print("Your word exists in the word list")
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

                if TestFlag == False: print("Suggestion %i: %s" % (i+1, updateNode.Word))
                elif TestFlag == True and i == 0: print("|%-25s|" % (updateNode.Word))
                elif TestFlag == True and i > 0: print("|%-30s|%-15s|%-25s|" % ("", "",  updateNode.Word))
        
        elif AlgorithmCheck == 0 and TestFlag == True: print("|%-25s|" % ("N/A"))
    
    elif WordFlag > 0 and TestFlag == True: print("|%-15s|%-25s|" % ("True", ""))


    #get the time of completion
    TimeTaken = (default_timer() - start) * 1000

    if TestFlag == False: print("Operation took: %f miliseconds to complete\n" % (TimeTaken))
    elif TestFlag == True: print("|Time to complete: %-10f ms%-41s|" %  (TimeTaken, ""))

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

#Optimal String Alignment Distance algorithm
def OptimalStringAlignmentDistance(string1 : str, string2 : str, len1 : int, len2 : int):
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

#Damerau-Levenshtein Distance algorithm
def DamerauLevenshteinDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    
    #make an array of the size of the alphabet for calculations
    alphabet = []

    for i in range (26): alphabet.append(0)

    #array to store results of calculations
    CalculationArray = [ [0 for x in range(len2+2)] for y in range (len1+2)]

    #get the maximum distance
    maxDist = len1 + len2
    CalculationArray[0][0] = maxDist

    #setup the starting values to the array
    for i in range(1, len1+1):
        CalculationArray[i][0] = maxDist
        CalculationArray[i][1] = i-1

    for j in range(1, len2+1):
        CalculationArray[0][j] = maxDist
        CalculationArray[1][j] = j-1

    #main calculation loop
    for i in range(len1):
        db = 0
        for j in range(len2):
            k = alphabet[ord(string2[j]) % 97]
            l = db
            cost = 0

            if string1[i] == string2[j]:
                cost = 0
                db = j

            else: cost = 1

            CalculationArray[i+2][j+2] = min( min(CalculationArray[i+1][j+1]+cost, #substitution
                                                    CalculationArray[i+2][j+1]+1 ), #insertion
                                              min(CalculationArray[i+1][j+2]+1, #deletion
                                                    CalculationArray[k][l]+(i-k+1)+1+(j-l+1)) ) #transpostion
        alphabet[ord(string1[i]) % 97] = i+2

    return CalculationArray[len1+1][len2+1]

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

        inputCheck = 1

        while inputCheck == 1:
            #user input
            ActionChoice = input(": ")

            #question mark denotes return to menu
            if ActionChoice ==  "?": return 0

            WordCheck = 0

            #check letters for non-alpha characters
            for letter in ActionChoice:
                if ord(letter) < 97 or ord(letter) > 122: WordCheck+=1

            #check that no errors occured, and that the length of the word is valid
            if WordCheck > 0 or len(ActionChoice) > 25:
                print("Please enter a valid word")
            else: inputCheck = 0

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
        linearCheck(WordArray, ActionChoice, 0, True)
        print("|------------------------------+---------------+-------------------------|")

        #Levenshtein Distance
        linearCheck(WordArray, ActionChoice, 1, True)
        print("|------------------------------+---------------+-------------------------|")

        #Hamming Distance
        linearCheck(WordArray, ActionChoice, 2, True)
        print("|------------------------------+---------------+-------------------------|")

        #Sørensen–Dice Coefficient
        linearCheck(WordArray, ActionChoice, 3, True)
        print("|------------------------------+---------------+-------------------------|")

        #Optimal String Alignment Distance
        linearCheck(WordArray, ActionChoice, 4, True)
        print("|------------------------------+---------------+-------------------------|")

        #Damerau-Levenshtein Distance
        linearCheck(WordArray, ActionChoice, 5, True)
        print("|------------------------------+---------------+-------------------------|")

        #Jaro Distance
        linearCheck(WordArray, ActionChoice, 6, True)
        print("|------------------------------+---------------+-------------------------|")

        #Jaro-Winkler Distance
        linearCheck(WordArray, ActionChoice, 6, True)
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
            binaryCheck(WordArray, ActionChoice, 0, True)
            print("|------------------------------+---------------+-------------------------|")

            #Levenshtein Distance
            binaryCheck(WordArray, ActionChoice, 1, True)
            print("|------------------------------+---------------+-------------------------|")

            #Hamming Distance
            binaryCheck(WordArray, ActionChoice, 2, True)
            print("|------------------------------+---------------+-------------------------|")

            #Sørensen–Dice Coefficient
            binaryCheck(WordArray, ActionChoice, 3, True)
            print("|------------------------------+---------------+-------------------------|")

            #Optimal String Alignment Distance
            binaryCheck(WordArray, ActionChoice, 4, True)
            print("|------------------------------+---------------+-------------------------|")

            #Damerau-Levenshtein Distance
            binaryCheck(WordArray, ActionChoice, 5, True)
            print("|------------------------------+---------------+-------------------------|")

            #Jaro Distance
            binaryCheck(WordArray, ActionChoice, 6, True)
            print("|------------------------------+---------------+-------------------------|")

            #Jaro-Winkler Distance
            binaryCheck(WordArray, ActionChoice, 7, True)
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
        hashCheck(WordHashTable, ActionChoice, 0, True)
        print("|------------------------------+---------------+-------------------------|")

        #Levenshtein Distance
        hashCheck(WordHashTable, ActionChoice, 1, True)
        print("|------------------------------+---------------+-------------------------|")

        #Hamming Distance
        hashCheck(WordHashTable, ActionChoice, 2, True)
        print("|------------------------------+---------------+-------------------------|")

        #Sørensen–Dice Coefficient
        hashCheck(WordHashTable, ActionChoice, 3, True)
        print("|------------------------------+---------------+-------------------------|")

        #Optimal String Alignment Distance
        hashCheck(WordHashTable, ActionChoice, 4, True)
        print("|------------------------------+---------------+-------------------------|")

        #Damerau-Levenshtein Distance
        hashCheck(WordHashTable, ActionChoice, 5, True)
        print("|------------------------------+---------------+-------------------------|")

        #Jaro Distance
        hashCheck(WordHashTable, ActionChoice, 6, True)
        print("|------------------------------+---------------+-------------------------|")

        #Jaro-Winkler Distance
        hashCheck(WordHashTable, ActionChoice, 7, True)

        print("--------------------------------------------------------------------------")
        print("\n")

        #user input (just to confirm that the user can return back to the previous menu)
        ActionChoice2 = input("Type anything to enter another word: ")

    return 0