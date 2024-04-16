import os
from timeit import default_timer
from bs4 import BeautifulSoup

#word file generation function
def DataReader(duplicateFlag : bool, sortFlag : bool, testFlag : bool):
    
    #clear the screen to reduce clutter
    os.system("cls")

    #show user options for the corpus
    print("Please type a letter from A to K as two which text you would like (has to be capitals):")
    print("Type Z, if you wish to return to the previous screen")

    letterCheck = 0
    ActionChoice = ""
    while letterCheck == 0:
        ActionChoice = input(": ")

        if len(ActionChoice) == 1 and ord(ActionChoice) >= 65 and ord(ActionChoice) <= 75:
            break
        elif ActionChoice == "Z" or ActionChoice == "z":
            return 0
        else:
            print("please input a single letter inside the required range and is capitalised")

    #set a timer
    start = default_timer()

    print("***Generating list***")
    print("LOADING...")

    WordArrayNoDuplicates = {}
    WordArrayDuplicates = []

    #this is the file where all the words are deposited
    if sortFlag == False and duplicateFlag == True:
        Wfp = open("./GeneratedFiles/LWD.txt", "w")
    elif sortFlag == True and duplicateFlag == True:
        Wfp = open("./GeneratedFiles/LWDS.txt", "w")
    elif sortFlag == False and duplicateFlag == False:
        Wfp = open("./GeneratedFiles/LWND.txt", "w")
    elif sortFlag == True and duplicateFlag == False:
        Wfp = open("./GeneratedFiles/LWNDS.txt", "w")
    else:
        print("Error: invalid sortCheck or duplicateCheck value given")
        return -1
    
    if Wfp == None:
        print("Error: unable to open word file")
        return -1
    
    #get the directory to search
    LetterDirectoryName = "../corpus/Texts/" + ActionChoice
    
    #wordcount measure
    WordCount = 0

    #get all the inner directories
    for folder in os.listdir(LetterDirectoryName):
        #add any inner folders to a new path
        InnerDirectoryName = LetterDirectoryName + "/" + folder
        #search new path for all files
        for file in os.listdir(InnerDirectoryName):
            FinalFilePath = InnerDirectoryName + "/" + file

            if testFlag == False:
                print(FinalFilePath)

            #open the file to read
            fp = open(FinalFilePath, "r")

            fileData = fp.read()

            #use beautiful soup to search for all the data in the file
            soup = BeautifulSoup(fileData, 'xml')
            
            #find all words in the xml file
            words = soup.find_all('w')

            #loop through all words in the file
            for word in words:
                #first, split words based ' characters
                firstSplit = word['hw'].split("'")
                
                for split1 in firstSplit:
                    #second, split words based on - 
                    secondSplit = split1.split("-")

                    for split2 in secondSplit:
                        #third, split based on /
                        thirdSplit = split2.split("/")

                        for split3 in thirdSplit:
                            #if word longer than 1 length allow it
                            if len(split3) > 1:
                                #duplicate check
                                if duplicateFlag == False:
                                    if split3 not in WordArrayNoDuplicates:
                                        #if not alread checked, add to list then add to file
                                        WordArrayNoDuplicates[split3] = 1
                                        WordCount+=1
                                else:
                                    WordArrayDuplicates.append(split3)
                                    WordCount+=1

            #close afterwards
            fp.close()

    print("**Loading Complete")
    print("WordCount: ", WordCount)

    if sortFlag == True:
        print("Sorting...")

        #depending on if there are duplicates or not, sort the relevant list
        #then write them to the file
        if duplicateFlag == True:
            WordArrayDuplicates = sorted(WordArrayDuplicates)
        else:
            WordArrayNoDuplicates = dict(sorted(WordArrayNoDuplicates.items()))

        print("**Sorting Completed")
    
    #then write them to the file
    if duplicateFlag == True:
        for word in WordArrayDuplicates:
            #print(word)
            Wfp.write(word)
            Wfp.write("\n")
    else:
        for word in WordArrayNoDuplicates.keys():
            #print(word)
            Wfp.write(word)
            Wfp.write("\n")

    #close afterward
    Wfp.close()

    TimeTaken = default_timer() - start
    print("Operation took: ",TimeTaken, "Seconds to complete\n")

    #user input (just to confirm that the user can return back to the previous menu)
    ActionChoice2 = input("Type anything to return: ")

    return 0
