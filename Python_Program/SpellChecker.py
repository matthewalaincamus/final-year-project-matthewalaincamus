import os
from timeit import default_timer

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
        if AlgorithmCheck == 0: print("|%-30s" % ("None"))
        elif AlgorithmCheck == 1: print("|%-30s" % ("LevenshteinDistance"))
        elif AlgorithmCheck == 2: print("|%-30s" % ("HammingDistance"))
        elif AlgorithmCheck == 3: print("|%-30s" % ("SorensenDiceCoefficient"))
        elif AlgorithmCheck == 4: print("|%-30s" % ("OptimalStringAlignmentDistance"))
        elif AlgorithmCheck == 5: print("|%-30s" % ("DamerauLevenshteinDistance"))
        elif AlgorithmCheck == 6: print("|%-30s" % ("JaroSimilarity"))
        elif AlgorithmCheck == 7: print("|%-30s" % ("JaroWinklerSimilarity"))

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
        elif TestFlag == True: print("|%-15s" % ("False"))

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
    TimeTaken = default_timer() - start

    if TestFlag == False: print("Operation took: %f secs to complete\n" % (TimeTaken))
    elif TestFlag == True: print("|Time to complete: %-10f secs%-41s|\n" %  (TimeTaken, ""))
    return 0

#perform binary search method
def binaryCheck(WordArray, wordString : str, AlgorithmCheck : int, TestFlag : bool):
    
    #set the timer
    start = default_timer()

    #if called from automated testing, print out data differently
    if TestFlag == True:
        #first print out algorithm used:
        if AlgorithmCheck == 0: print("|%-30s" % ("None"))
        elif AlgorithmCheck == 1: print("|%-30s" % ("LevenshteinDistance"))
        elif AlgorithmCheck == 2: print("|%-30s" % ("HammingDistance"))
        elif AlgorithmCheck == 3: print("|%-30s" % ("SorensenDiceCoefficient"))
        elif AlgorithmCheck == 4: print("|%-30s" % ("OptimalStringAlignmentDistance"))
        elif AlgorithmCheck == 5: print("|%-30s" % ("DamerauLevenshteinDistance"))
        elif AlgorithmCheck == 6: print("|%-30s" % ("JaroSimilarity"))
        elif AlgorithmCheck == 7: print("|%-30s" % ("JaroWinklerSimilarity"))

    binaryChecker = binarySearch(WordArray, 0, len(WordArray) - 1, wordString)
    if binaryChecker == 0:
        if TestFlag == False: print("Your word doesn't exist in the word list")
        elif TestFlag == True: print("|%-15s" % ("False"))

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

        elif binaryChecker == 1:
            if TestFlag == 0: print("Your word exists in the word list")
            elif TestFlag == 1: print("|%-15s|%-25s|" % ("True", ""))

        else:
            print("Error: problem with binary search")

    #get the time of completion
    TimeTaken = default_timer() - start

    if TestFlag == False: print("Operation took: %f secs to complete\n" % (TimeTaken))
    elif TestFlag == True: print("|Time to complete: %-10f secs%-41s|\n" %  (TimeTaken, ""))

    return 0

#perform binary search algorithm (recursive)
def binarySearch(WordArray, leftValue : int, rightValue : int, wordString : str):
    
    if rightValue >= leftValue:

        middleValue = leftValue + (rightValue - leftValue) / 2

        if wordString == WordArray[middleValue]: return 1
        elif wordString < WordArray[middleValue]: return binarySearch(WordArray, leftValue, middleValue - 1, wordString)
        elif wordString > WordArray[middleValue]: return binarySearch(WordArray, middleValue + 1, rightValue, wordString)
        
    return 0

#perform hash table search method
def hashCheck(HashTable : HashTable, wordString : str, AlgorithmCheck : int, TestFlag : bool ):
    
    #set the timer
    start = default_timer()

    #if called from automated testing, print out data differently
    if TestFlag == True:
        #first print out algorithm used:
        if AlgorithmCheck == 0: print("|%-30s" % ("None"))
        elif AlgorithmCheck == 1: print("|%-30s" % ("LevenshteinDistance"))
        elif AlgorithmCheck == 2: print("|%-30s" % ("HammingDistance"))
        elif AlgorithmCheck == 3: print("|%-30s" % ("SorensenDiceCoefficient"))
        elif AlgorithmCheck == 4: print("|%-30s" % ("OptimalStringAlignmentDistance"))
        elif AlgorithmCheck == 5: print("|%-30s" % ("DamerauLevenshteinDistance"))
        elif AlgorithmCheck == 6: print("|%-30s" % ("JaroSimilarity"))
        elif AlgorithmCheck == 7: print("|%-30s" % ("JaroWinklerSimilarity"))
    
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
        elif TestFlag == 1: print("|%-15s" % ("False"))

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
                    while currentNode.next is not None:
                        #if an algorithm is active, get the distance between the two words
                        if AlgorithmCheck == 1: currentNode.AlgorithmDistance = LevenshteinDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 2: currentNode.AlgorithmDistance = HammingDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 3: currentNode.AlgorithmDistance = SorensenDiceCoefficient(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 4: currentNode.AlgorithmDistance = OptimalStringAlignmentDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 5: currentNode.AlgorithmDistance = DamerauLevenshteinDistance(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 6: currentNode.AlgorithmDistance = JaroSimilarity(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        elif AlgorithmCheck == 7: currentNode.AlgorithmDistance = JaroWinklerSimilarity(wordString, currentNode.Word, len(wordString), len(currentNode.Word))
                        currentNode = currentNode.next
    
            #get the top 10 suggestions for the user word
            for i in range(10):
                currentMin = 100
                currentMinIndex = 0
                currentMinKeyIndex = 0

                for i in range (HashTable.maxSize):
                    if HashTable.table[i] is not None:
                        currentNode = HashTable.table[i]

                        #if there is only one node in a linked list
                        if currentNode.next is None:
                            if currentNode.AlgorithmDistance < currentMin:
                                currentMin = currentNode.AlgorithmDistance
                                currentMinIndex = i
                                currentMinKeyIndex = currentNode.key
        
                        #if there is more than one node
                        while currentNode.next is not None:                            
                            currentNode = currentNode.next

                            if currentNode.AlgorithmDistance < currentMin:
                                currentMin = currentNode.AlgorithmDistance
                                currentMinIndex = i
                                currentMinKeyIndex = currentNode.key
                            
                
                #update the min node to be large
                updateNode = HashTable.table[currentMinIndex]
                for i in range(currentMinKeyIndex):
                    updateNode = updateNode.next

                updateNode.AlgorithmDistance = 100

                if TestFlag == False: print("Suggestion %i: %s" % (i+1, updateNode.Word))
                elif TestFlag == True and i == 0: print("|%-25s|" % (updateNode.Word))
                elif TestFlag == True and i > 0: print("|%-30s|%-15s|%-25s|" % ("", "",  updateNode.Word))
        
        elif AlgorithmCheck == 0 and TestFlag == True: print("|%-25s|" % ("N/A"))
    
    elif WordFlag > 0 and TestFlag == True: print("|%-15s|%-25s|" % ("True", ""))


    #get the time of completion
    TimeTaken = default_timer() - start

    if TestFlag == False: print("Operation took: %f secs to complete\n" % (TimeTaken))
    elif TestFlag == True: print("|Time to complete: %-10f secs%-41s|\n" %  (TimeTaken, ""))

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

        #update the word count of the hash table
        WordHashTable.wordCount += 1

    return WordHashTable

#Levenshtein Distance algorithm
def LevenshteinDistance(string1 : str, string2 : str, len1 : int, len2 : int):
    
    #empty string1
    if len1 == 0: return len2

    #empty string2
    if len2 == 0: return len1

    if string1[len1 - 1] == string2[len2 - 1]:
        return LevenshteinDistance(string1, string2, len1 - 1, len2 - 1)
    
    return 1 + min(
        LevenshteinDistance(string1, string2, len1, len2 -1), #insert
        min( 
            LevenshteinDistance(string1, string2, len1 - 1, len2), #remove
            LevenshteinDistance(string1, string2, len1 - 1, len2 - 1) #replace
        ))

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
    #number of bigraphs that match
    matches = 0.0

    i = 0
    j = 0

    #loop through, getting the number of bigraphs that match and updating the count
    while i < len1 - 1 and j < len2 - 1:
        bigraph1 = "" + string1[i] + string2[i+1]
        bigraph2 = "" + string2[j] + string2[j+1]

        if bigraph1 == bigraph2: matches+=1

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
    CalculationArray = [len1+1][len2+1]

    #add starting values to calculation array
    for i in range(len1 + 1): CalculationArray[i][0] = i
    for j in range(len2 + 1): CalculationArray[0][j] = j

    for i in range(len1 + 1):
        for j in range(len2 + 1):
            cost = 0

            if string1[i] == string2[j]: cost = 0
            else: cost = 1

            CalculationArray[i+1][j+1] = min(CalculationArray[i][j+1] + 1, #deletion
                                            CalculationArray[i+1][j] + 1, #insertion
                                            CalculationArray[i][j] + cost) #substitution
            
            if i > 0 and j > 0 and string1[i+1] == string2[j] and string1[i] == string2[j+1]:
                CalculationArray[i][j] = min(CalculationArray[i][j],
                                                CalculationArray[i-1][j-1] + 1) #transposition

    return CalculationArray[len1][len2]

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