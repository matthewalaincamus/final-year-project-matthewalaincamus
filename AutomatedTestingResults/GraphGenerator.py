import matplotlib.pyplot as plt
import numpy as np
import os

def fileParser(FilePath : str, type : int):
    fp = open(FilePath, "r")

    Lines = fp.readlines()

    fp.close()

    returnDictionary = {}

    LineCount = 0

    for line in Lines:
        #print(line[:-1])

        #for first line, store the correct and incorrect counts
        if LineCount == 0:
            #print(int(line[14:18]))
            #print(int(line[36:41]))

            Counts = line[:-1].split(',')
            for count in Counts:
                Numbers = count[1:].split(' ')
                if Numbers[0] == "orrectCount:":
                    returnDictionary["CorrectCount"] = int(Numbers[1])
                else:
                    returnDictionary["IncorrectCount"] = int(Numbers[1])

        #for the third line onwards, store the algorithm times and suggestion numbers
        if LineCount >= 2:
            #print(line[2:4])
            if line[2:4] == "NO":
                if line[0:1] == "L":
                    if "Linear No-Algorithm TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Linear No-Algorithm TimeTaken"] = []
                    returnDictionary["Linear No-Algorithm TimeTaken"].append(float(line[6:-1]))
                elif line[0:1] == "B":
                    if "Binary No-Algorithm TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Binary No-Algorithm TimeTaken"] = []
                    returnDictionary["Binary No-Algorithm TimeTaken"].append(float(line[6:-1]))
                elif line[0:1] == "H":
                    if "Hash No-Algorithm TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Hash No-Algorithm TimeTaken"] = []
                    returnDictionary["Hash No-Algorithm TimeTaken"].append(float(line[6:-1]))

            elif line[2:4] == "LD":

                divided = line.split(":")
                #print(divided[1][1:-1], divided[2][1:-1])

                if line[0:1] == "L":
                    if "Linear Levenshtein-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Linear Levenshtein-Distance TimeTaken"] = []
                    if "Linear Levenshtein-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Linear Levenshtein-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Linear Levenshtein-Distance SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Linear Levenshtein-Distance TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "B":
                    if "Binary Levenshtein-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Binary Levenshtein-Distance TimeTaken"] = []
                    if "Binary Levenshtein-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Binary Levenshtein-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Binary Levenshtein-Distance SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Binary Levenshtein-Distance TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "H":
                    if "Hash Levenshtein-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Hash Levenshtein-Distance TimeTaken"] = []
                    if "Hash Levenshtein-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Hash Levenshtein-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Hash Levenshtein-Distance SuggestionNumber"].append(int(divided[1][1:-1]))  
                    returnDictionary["Hash Levenshtein-Distance TimeTaken"].append(float(divided[2][1:-1]))
        
            elif line[2:4] == "HD":

                divided = line.split(":")
                #print(divided[1][1:-1], divided[2][1:-1])

                if line[0:1] == "L":
                    if "Linear Hamming-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Linear Hamming-Distance TimeTaken"] = []
                    if "Linear Hamming-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Linear Hamming-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Linear Hamming-Distance SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Linear Hamming-Distance TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "B":
                    if "Binary Hamming-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Binary Hamming-Distance TimeTaken"] = []
                    if "Binary Hamming-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Binary Hamming-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Binary Hamming-Distance SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Binary Hamming-Distance TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "H":
                    if "Hash Hamming-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Hash Hamming-Distance TimeTaken"] = []
                    if "Hash Hamming-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Hash Hamming-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Hash Hamming-Distance SuggestionNumber"].append(int(divided[1][1:-1]))  
                    returnDictionary["Hash Hamming-Distance TimeTaken"].append(float(divided[2][1:-1]))
            
            elif line[2:4] == "SD":

                divided = line.split(":")
                #print(divided[1][1:-1], divided[2][1:-1])

                if line[0:1] == "L":
                    if "Linear Sorensen-Dice TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Linear Sorensen-Dice TimeTaken"] = []
                    if "Linear Sorensen-Dice SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Linear Sorensen-Dice SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Linear Sorensen-Dice SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Linear Sorensen-Dice TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "B":
                    if "Binary Sorensen-Dice TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Binary Sorensen-Dice TimeTaken"] = []
                    if "Binary Sorensen-Dice SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Binary Sorensen-Dice SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Binary Sorensen-Dice SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Binary Sorensen-Dice TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "H":
                    if "Hash Sorensen-Dice TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Hash Sorensen-Dice TimeTaken"] = []
                    if "Hash Sorensen-Dice SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Hash Sorensen-Dice SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Hash Sorensen-Dice SuggestionNumber"].append(int(divided[1][1:-1]))  
                    returnDictionary["Hash Sorensen-Dice TimeTaken"].append(float(divided[2][1:-1]))
            
            elif line[2:4] == "OS":

                divided = line.split(":")
                #print(divided[1][1:-1], divided[2][1:-1])

                if line[0:1] == "L":
                    if "Linear Optimal-String-Alignment TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Linear Optimal-String-Alignment TimeTaken"] = []
                    if "Linear Optimal-String-Alignment SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Linear Optimal-String-Alignment SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Linear Optimal-String-Alignment SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Linear Optimal-String-Alignment TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "B":
                    if "Binary Optimal-String-Alignment TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Binary Optimal-String-Alignment TimeTaken"] = []
                    if "Binary Optimal-String-Alignment SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Binary Optimal-String-Alignment SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Binary Optimal-String-Alignment SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Binary Optimal-String-Alignment TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "H":
                    if "Hash Optimal-String-Alignment TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Hash Optimal-String-Alignment TimeTaken"] = []
                    if "Hash Optimal-String-Alignment SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Hash Optimal-String-Alignment SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Hash Optimal-String-Alignment SuggestionNumber"].append(int(divided[1][1:-1]))  
                    returnDictionary["Hash Optimal-String-Alignment TimeTaken"].append(float(divided[2][1:-1]))

            elif line[2:4] == "DL":

                divided = line.split(":")
                #print(divided[1][1:-1], divided[2][1:-1])

                if line[0:1] == "L":
                    if "Linear Damerau-Levenshtein TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Linear Damerau-Levenshtein TimeTaken"] = []
                    if "Linear Damerau-Levenshtein SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Linear Damerau-Levenshtein SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Linear Damerau-Levenshtein SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Linear Damerau-Levenshtein TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "B":
                    if "Binary Damerau-Levenshtein TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Binary Damerau-Levenshtein TimeTaken"] = []
                    if "Binary Damerau-Levenshtein SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Binary Damerau-Levenshtein SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Binary Damerau-Levenshtein SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Binary Damerau-Levenshtein TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "H":
                    if "Hash Damerau-Levenshtein TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Hash Damerau-Levenshtein TimeTaken"] = []
                    if "Hash Damerau-Levenshtein SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Hash Damerau-Levenshtein SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Hash Damerau-Levenshtein SuggestionNumber"].append(int(divided[1][1:-1]))  
                    returnDictionary["Hash Damerau-Levenshtein TimeTaken"].append(float(divided[2][1:-1]))
            
            elif line[2:4] == "JD":

                divided = line.split(":")
                #print(divided[1][1:-1], divided[2][1:-1])

                if line[0:1] == "L":
                    if "Linear Jaro-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Linear Jaro-Distance TimeTaken"] = []
                    if "Linear Jaro-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Linear Jaro-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Linear Jaro-Distance SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Linear Jaro-Distance TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "B":
                    if "Binary Jaro-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Binary Jaro-Distance TimeTaken"] = []
                    if "Binary Jaro-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Binary Jaro-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Binary Jaro-Distance SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Binary Jaro-Distance TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "H":
                    if "Hash Jaro-Distance TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Hash Jaro-Distance TimeTaken"] = []
                    if "Hash Jaro-Distance SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Hash Jaro-Distance SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Hash Jaro-Distance SuggestionNumber"].append(int(divided[1][1:-1]))  
                    returnDictionary["Hash Jaro-Distance TimeTaken"].append(float(divided[2][1:-1]))

            elif line[2:4] == "JW":

                divided = line.split(":")
                #print(divided[1][1:-1], divided[2][1:-1])

                if line[0:1] == "L":
                    if "Linear Jaro-Winkler TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Linear Jaro-Winkler TimeTaken"] = []
                    if "Linear Jaro-Winkler SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Linear Jaro-Winkler SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Linear Jaro-Winkler SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Linear Jaro-Winkler TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "B":
                    if "Binary Jaro-Winkler TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Binary Jaro-Winkler TimeTaken"] = []
                    if "Binary Jaro-Winkler SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Binary Jaro-Winkler SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Binary Jaro-Winkler SuggestionNumber"].append(int(divided[1][1:-1]))
                    returnDictionary["Binary Jaro-Winkler TimeTaken"].append(float(divided[2][1:-1]))
                elif line[0:1] == "H":
                    if "Hash Jaro-Winkler TimeTaken" not in returnDictionary.keys():
                        returnDictionary["Hash Jaro-Winkler TimeTaken"] = []
                    if "Hash Jaro-Winkler SuggestionNumber" not in returnDictionary.keys():
                        returnDictionary["Hash Jaro-Winkler SuggestionNumber"] = []
                    if int(divided[1][1:-1]) != 0:
                        returnDictionary["Hash Jaro-Winkler SuggestionNumber"].append(int(divided[1][1:-1]))  
                    returnDictionary["Hash Jaro-Winkler TimeTaken"].append(float(divided[2][1:-1]))
            
            elif line[0:2] == "LD":

                division = line[4:-1].split(", ")
                
                returnDictionary["Levenshtein-Distance Miss Counts"] = [int(division[0][2:]),
                                                                        int(division[1][2:]),
                                                                        int(division[2][2:])]
            elif line[0:2] == "HD":

                division = line[4:-1].split(", ")
                
                returnDictionary["Hamming-Distance Miss Counts"] = [int(division[0][2:]),
                                                                    int(division[1][2:]),
                                                                    int(division[2][2:])]
            elif line[0:2] == "SD":

                division = line[4:-1].split(", ")
                
                returnDictionary["Sorensen-Dice Miss Counts"] = [int(division[0][2:]),
                                                                int(division[1][2:]),
                                                                int(division[2][2:])]
            elif line[0:2] == "OS":

                division = line[4:-1].split(", ")
                
                returnDictionary["Optimal-String-Alignment Miss Counts"] = [int(division[0][2:]),
                                                                            int(division[1][2:]),
                                                                            int(division[2][2:])]
            elif line[0:2] == "DL":

                division = line[4:-1].split(", ")
                
                returnDictionary["Damerau-Levenshtein Miss Counts"] = [int(division[0][2:]),
                                                                        int(division[1][2:]),
                                                                        int(division[2][2:])]
            elif line[0:2] == "JD":

                division = line[4:-1].split(", ")
                
                returnDictionary["Jaro-Distance Miss Counts"] = [int(division[0][2:]),
                                                                int(division[1][2:]),
                                                                int(division[2][2:])]
            elif line[0:2] == "JW":

                division = line[4:-1].split(", ")
                
                returnDictionary["Jaro-Winkler Miss Counts"] = [int(division[0][2:]),
                                                                int(division[1][2:]),
                                                                int(division[2][2:])]

                
        LineCount+=1
    
    '''
    print("CorrectCount",returnDictionary["CorrectCount"], "IncorrectCount", returnDictionary["IncorrectCount"])
    #no algorithm
    print("L/NO",len(returnDictionary["Linear No-Algorithm TimeTaken"]))
    try: print("B/NO",len(returnDictionary["Binary No-Algorithm TimeTaken"]))
    except KeyError: print("B/NO", None)
    print("H/NO", len(returnDictionary["Hash No-Algorithm TimeTaken"]))
    #Levenshtein
    print("L/LD",len(returnDictionary["Linear Levenshtein-Distance TimeTaken"]), len(returnDictionary["Linear Levenshtein-Distance SuggestionNumber"]))
    try: print("B/LD",len(returnDictionary["Binary Levenshtein-Distance TimeTaken"]), len(returnDictionary["Binary Levenshtein-Distance SuggestionNumber"]))
    except KeyError: print("B/LD", None)
    print("H/LD", len(returnDictionary["Hash Levenshtein-Distance TimeTaken"]), len(returnDictionary["Hash Levenshtein-Distance SuggestionNumber"]))
    #Hamming
    print("L/HD",len(returnDictionary["Linear Hamming-Distance TimeTaken"]), len(returnDictionary["Linear Hamming-Distance SuggestionNumber"]))
    try: print("B/HD",len(returnDictionary["Binary Hamming-Distance TimeTaken"]), len(returnDictionary["Binary Hamming-Distance SuggestionNumber"]))
    except KeyError: print("B/HD", None)
    print("H/HD", len(returnDictionary["Hash Hamming-Distance TimeTaken"]), len(returnDictionary["Hash Hamming-Distance SuggestionNumber"]))
    #Sorensen
    print("L/SD",len(returnDictionary["Linear Sorensen-Dice TimeTaken"]), len(returnDictionary["Linear Sorensen-Dice SuggestionNumber"]))
    try: print("B/SD",len(returnDictionary["Binary Sorensen-Dice TimeTaken"]), len(returnDictionary["Binary Sorensen-Dice SuggestionNumber"]))
    except KeyError: print("B/SD", None)
    print("H/SD", len(returnDictionary["Hash Sorensen-Dice TimeTaken"]), len(returnDictionary["Hash Sorensen-Dice SuggestionNumber"]))
    #Optimal
    print("L/OS",len(returnDictionary["Linear Optimal-String-Alignment TimeTaken"]), len(returnDictionary["Linear Optimal-String-Alignment SuggestionNumber"]))
    try: print("B/OS",len(returnDictionary["Binary Optimal-String-Alignment TimeTaken"]), len(returnDictionary["Binary Optimal-String-Alignment SuggestionNumber"]))
    except KeyError: print("B/OS", None)
    print("H/OS", len(returnDictionary["Hash Optimal-String-Alignment TimeTaken"]), len(returnDictionary["Hash Optimal-String-Alignment SuggestionNumber"]))
    #Damerau
    print("L/DL",len(returnDictionary["Linear Damerau-Levenshtein TimeTaken"]), len(returnDictionary["Linear Damerau-Levenshtein SuggestionNumber"]))
    try: print("B/DL",len(returnDictionary["Binary Damerau-Levenshtein TimeTaken"]), len(returnDictionary["Binary Damerau-Levenshtein SuggestionNumber"]))
    except KeyError: print("B/DL", None)
    print("H/DL", len(returnDictionary["Hash Damerau-Levenshtein TimeTaken"]), len(returnDictionary["Hash Damerau-Levenshtein SuggestionNumber"]))
    #Jaro
    print("L/JD",len(returnDictionary["Linear Jaro-Distance TimeTaken"]), len(returnDictionary["Linear Jaro-Distance SuggestionNumber"]))
    try: print("B/JD",len(returnDictionary["Binary Jaro-Distance TimeTaken"]), len(returnDictionary["Binary Jaro-Distance SuggestionNumber"]))
    except KeyError: print("B/JD", None)
    print("H/JD", len(returnDictionary["Hash Jaro-Distance TimeTaken"]), len(returnDictionary["Hash Jaro-Distance SuggestionNumber"]))
    #JaroWinkler
    print("L/JW",len(returnDictionary["Linear Jaro-Winkler TimeTaken"]), len(returnDictionary["Linear Jaro-Winkler SuggestionNumber"]))
    try: print("B/JW",len(returnDictionary["Binary Jaro-Winkler TimeTaken"]), len(returnDictionary["Binary Jaro-Winkler SuggestionNumber"]))
    except KeyError: print("B/JW", None)
    print("H/JW", len(returnDictionary["Hash Jaro-Winkler TimeTaken"]), len(returnDictionary["Hash Jaro-Winkler SuggestionNumber"]))
    '''
    return returnDictionary

def graphingGenerator(FileData : dict):

    #time taken boxplots by method

    #*linear:
    L_NO = np.array(FileData["Linear No-Algorithm TimeTaken"])
    L_LD = np.array(FileData["Linear Levenshtein-Distance TimeTaken"])
    L_HD = np.array(FileData["Linear Hamming-Distance TimeTaken"])
    L_SD = np.array(FileData["Linear Sorensen-Dice TimeTaken"])
    L_OS = np.array(FileData["Linear Optimal-String-Alignment TimeTaken"])
    L_DL = np.array(FileData["Linear Damerau-Levenshtein TimeTaken"])
    L_JD = np.array(FileData["Linear Jaro-Distance TimeTaken"])
    L_JW = np.array(FileData["Linear Jaro-Winkler TimeTaken"])
    dataLinear = [L_NO, L_LD, L_HD, L_SD, L_OS, L_DL, L_JD, L_JW]

    figLinear, ax1 = plt.subplots()
    ax1.boxplot(dataLinear, showfliers=False)
    plt.xticks([1,2,3,4,5,6,7,8], ["N/A", 
                                    "Levenshtein Distance",
                                    "Hamming Distance",
                                    "Sorensen-Dice",
                                    "Optimal String Alignment",
                                    "Damerau Levenshtein",
                                    "Jaro Distance",
                                    "Jaro Winkler"])
    plt.grid()
    plt.title("Linear Search Algorithm Runtimes with %s words (outliers excluded)" % (FileData["IncorrectCount"]))
    plt.xlabel("Algorithms")
    plt.ylabel("Runtime (ms)")
    

    plt.show()

    #*binary:
    try:
        B_NO = np.array(FileData["Binary No-Algorithm TimeTaken"])
        B_LD = np.array(FileData["Binary Levenshtein-Distance TimeTaken"])
        B_HD = np.array(FileData["Binary Hamming-Distance TimeTaken"])
        B_SD = np.array(FileData["Binary Sorensen-Dice TimeTaken"])
        B_OS = np.array(FileData["Binary Optimal-String-Alignment TimeTaken"])
        B_DL = np.array(FileData["Binary Damerau-Levenshtein TimeTaken"])
        B_JD = np.array(FileData["Binary Jaro-Distance TimeTaken"])
        B_JW = np.array(FileData["Binary Jaro-Winkler TimeTaken"])
        dataBinary = [B_NO, B_LD, B_HD, B_SD, B_OS, B_DL, B_JD, B_JW]

        figBinary, ax2 = plt.subplots()
        ax2.boxplot(dataBinary, showfliers=False)
        plt.xticks([1,2,3,4,5,6,7,8], ["N/A", 
                                        "Levenshtein Distance",
                                        "Hamming Distance",
                                        "Sorensen-Dice",
                                        "Optimal String Alignment",
                                        "Damerau Levenshtein",
                                        "Jaro Distance",
                                        "Jaro Winkler"])
        plt.grid()
        plt.title("Binary Search Algorithm Runtimes with %s words (outliers excluded)" % (FileData["IncorrectCount"]))
        plt.xlabel("Algorithms")
        plt.ylabel("Runtime (ms)")

        plt.show()
    except KeyError:
        print(None)

    #*hashing
    H_NO = np.array(FileData["Hash No-Algorithm TimeTaken"])
    H_LD = np.array(FileData["Hash Levenshtein-Distance TimeTaken"])
    H_HD = np.array(FileData["Hash Hamming-Distance TimeTaken"])
    H_SD = np.array(FileData["Hash Sorensen-Dice TimeTaken"])
    H_OS = np.array(FileData["Hash Optimal-String-Alignment TimeTaken"])
    H_DL = np.array(FileData["Hash Damerau-Levenshtein TimeTaken"])
    H_JD = np.array(FileData["Hash Jaro-Distance TimeTaken"])
    H_JW = np.array(FileData["Hash Jaro-Winkler TimeTaken"])
    dataHash = [H_NO, H_LD, H_HD, H_SD, H_OS, H_DL, H_JD, H_JW]

    figHash, ax3 = plt.subplots()
    ax3.boxplot(dataHash, showfliers=False)
    plt.xticks([1,2,3,4,5,6,7,8], ["N/A", 
                                    "Levenshtein Distance",
                                    "Hamming Distance",
                                    "Sorensen-Dice",
                                    "Optimal String Alignment",
                                    "Damerau Levenshtein",
                                    "Jaro Distance",
                                    "Jaro Winkler"])
    plt.grid()
    plt.title("Hash Search Algorithm Runtimes with %s words (outliers excluded)" % (FileData["IncorrectCount"]))
    plt.xlabel("Algorithms")
    plt.ylabel("Runtime (ms)")

    plt.show()

    #suggestion number boxplots by method

    #*linear:
    L_LD = np.array(FileData["Linear Levenshtein-Distance SuggestionNumber"])
    L_HD = np.array(FileData["Linear Hamming-Distance SuggestionNumber"])
    L_SD = np.array(FileData["Linear Sorensen-Dice SuggestionNumber"])
    L_OS = np.array(FileData["Linear Optimal-String-Alignment SuggestionNumber"])
    L_DL = np.array(FileData["Linear Damerau-Levenshtein SuggestionNumber"])
    L_JD = np.array(FileData["Linear Jaro-Distance SuggestionNumber"])
    L_JW = np.array(FileData["Linear Jaro-Winkler SuggestionNumber"])
    dataLinear = [L_LD, L_HD, L_SD, L_OS, L_DL, L_JD, L_JW]

    figLinear, ax1 = plt.subplots()
    ax1.boxplot(dataLinear)
    plt.xticks([1,2,3,4,5,6,7], ["Levenshtein Distance",
                                    "Hamming Distance",
                                    "Sorensen-Dice",
                                    "Optimal String Alignment",
                                    "Damerau Levenshtein",
                                    "Jaro Distance",
                                    "Jaro Winkler"])
    plt.grid()
    plt.title("Linear Search Algorithm Suggestion Numbers with %s words (outliers included)" % (FileData["IncorrectCount"]))
    plt.xlabel("Algorithms")
    plt.ylabel("Suggestion Numbers")
    

    plt.show()

    #*binary:
    try:
        B_LD = np.array(FileData["Binary Levenshtein-Distance SuggestionNumber"])
        B_HD = np.array(FileData["Binary Hamming-Distance SuggestionNumber"])
        B_SD = np.array(FileData["Binary Sorensen-Dice SuggestionNumber"])
        B_OS = np.array(FileData["Binary Optimal-String-Alignment SuggestionNumber"])
        B_DL = np.array(FileData["Binary Damerau-Levenshtein SuggestionNumber"])
        B_JD = np.array(FileData["Binary Jaro-Distance SuggestionNumber"])
        B_JW = np.array(FileData["Binary Jaro-Winkler SuggestionNumber"])
        dataBinary = [B_LD, B_HD, B_SD, B_OS, B_DL, B_JD, B_JW]

        figBinary, ax2 = plt.subplots()
        ax2.boxplot(dataBinary)
        plt.xticks([1,2,3,4,5,6,7], ["Levenshtein Distance",
                                        "Hamming Distance",
                                        "Sorensen-Dice",
                                        "Optimal String Alignment",
                                        "Damerau Levenshtein",
                                        "Jaro Distance",
                                        "Jaro Winkler"])
        plt.grid()
        plt.title("Binary Search Algorithm Suggestion Numbers with %s words (outliers included)" % (FileData["IncorrectCount"]))
        plt.xlabel("Algorithms")
        plt.ylabel("Suggestion Numbers")

        plt.show()
    except KeyError:
        print(None)

    #*hashing
    H_LD = np.array(FileData["Hash Levenshtein-Distance SuggestionNumber"])
    H_HD = np.array(FileData["Hash Hamming-Distance SuggestionNumber"])
    H_SD = np.array(FileData["Hash Sorensen-Dice SuggestionNumber"])
    H_OS = np.array(FileData["Hash Optimal-String-Alignment SuggestionNumber"])
    H_DL = np.array(FileData["Hash Damerau-Levenshtein SuggestionNumber"])
    H_JD = np.array(FileData["Hash Jaro-Distance SuggestionNumber"])
    H_JW = np.array(FileData["Hash Jaro-Winkler SuggestionNumber"])
    dataHash = [H_LD, H_HD, H_SD, H_OS, H_DL, H_JD, H_JW]

    figHash, ax3 = plt.subplots()
    ax3.boxplot(dataHash)
    plt.xticks([1,2,3,4,5,6,7], ["Levenshtein Distance",
                                    "Hamming Distance",
                                    "Sorensen-Dice",
                                    "Optimal String Alignment",
                                    "Damerau Levenshtein",
                                    "Jaro Distance",
                                    "Jaro Winkler"])
    plt.grid()
    plt.title("Hash Search Algorithm Suggestion Numbers with %d words (outliers included)" % (FileData["IncorrectCount"]))
    plt.xlabel("Algorithms")
    plt.ylabel("Suggestion Numbers")

    plt.show()

    #miss count pie charts
   
    #*linear
    LD = np.array([ FileData["Levenshtein-Distance Miss Counts"][0], 
                  FileData["IncorrectCount"] - FileData["Levenshtein-Distance Miss Counts"][0]])
    HD = np.array([FileData["Hamming-Distance Miss Counts"][0], 
                  FileData["IncorrectCount"] - FileData["Hamming-Distance Miss Counts"][0]])
    SD = np.array([FileData["Sorensen-Dice Miss Counts"][0],
                  FileData["IncorrectCount"] - FileData["Sorensen-Dice Miss Counts"][0]])
    OS = np.array([FileData["Optimal-String-Alignment Miss Counts"][0],
                  FileData["IncorrectCount"] - FileData["Optimal-String-Alignment Miss Counts"][0]])
    DL = np.array([FileData["Damerau-Levenshtein Miss Counts"][0],
                  FileData["IncorrectCount"] - FileData["Damerau-Levenshtein Miss Counts"][0]])
    JD = np.array([FileData["Jaro-Distance Miss Counts"][0],
                  FileData["IncorrectCount"] - FileData["Jaro-Distance Miss Counts"][0]])
    JW = np.array([FileData["Jaro-Winkler Miss Counts"][0],
                  FileData["IncorrectCount"] - FileData["Jaro-Winkler Miss Counts"][0]])

    fig, axes = plt.subplots(2,4)
    plt.suptitle("The ratio of Misses to Non-Misses of spelling %d words with the Linear Search Method" % ((FileData["IncorrectCount"])))

    PieLabels = ["Misses", "Non-Misses"]
    axes[0,0].pie(LD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[0,0].title.set_text("Levenshtein Distance")
    axes[0,1].pie(HD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[0,1].title.set_text("Hamming Distance")
    axes[0,2].pie(SD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[0,2].title.set_text("Sorensen-Dice Distance")
    axes[0,3].pie(OS, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[0,3].title.set_text("Optimal-String-Alignment")
    axes[1,0].pie(DL, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[1,0].title.set_text("Damereau Levenshtein")
    axes[1,1].pie(JD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[1,1].title.set_text("Jaro Distance")
    axes[1,2].pie(JW, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[1,2].title.set_text("Jaro Winkler")
    fig.delaxes(ax=axes[1,3])
    plt.show()

    #*binary
    if FileData["Hamming-Distance Miss Counts"][1] != 0:
        LD = np.array([ FileData["Levenshtein-Distance Miss Counts"][1], 
                    FileData["IncorrectCount"] - FileData["Levenshtein-Distance Miss Counts"][1]])
        HD = np.array([FileData["Hamming-Distance Miss Counts"][1], 
                    FileData["IncorrectCount"] - FileData["Hamming-Distance Miss Counts"][1]])
        SD = np.array([FileData["Sorensen-Dice Miss Counts"][1],
                    FileData["IncorrectCount"] - FileData["Sorensen-Dice Miss Counts"][1]])
        OS = np.array([FileData["Optimal-String-Alignment Miss Counts"][1],
                    FileData["IncorrectCount"] - FileData["Optimal-String-Alignment Miss Counts"][1]])
        DL = np.array([FileData["Damerau-Levenshtein Miss Counts"][1],
                    FileData["IncorrectCount"] - FileData["Damerau-Levenshtein Miss Counts"][1]])
        JD = np.array([FileData["Jaro-Distance Miss Counts"][1],
                    FileData["IncorrectCount"] - FileData["Jaro-Distance Miss Counts"][1]])
        JW = np.array([FileData["Jaro-Winkler Miss Counts"][1],
                    FileData["IncorrectCount"] - FileData["Jaro-Winkler Miss Counts"][1]])

        fig, axes = plt.subplots(2,4)
        plt.suptitle("The ratio of Misses to Non-Misses of spelling %d words with the Binary Search Method" % ((FileData["IncorrectCount"])))

        PieLabels = ["Misses", "Non-Misses"]
        axes[0,0].pie(LD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
        axes[0,0].title.set_text("Levenshtein Distance")
        axes[0,1].pie(HD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
        axes[0,1].title.set_text("Hamming Distance")
        axes[0,2].pie(SD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
        axes[0,2].title.set_text("Sorensen-Dice Distance")
        axes[0,3].pie(OS, labels=PieLabels, shadow=True, autopct='%1.2f%%')
        axes[0,3].title.set_text("Optimal-String-Alignment")
        axes[1,0].pie(DL, labels=PieLabels, shadow=True, autopct='%1.2f%%')
        axes[1,0].title.set_text("Damereau Levenshtein")
        axes[1,1].pie(JD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
        axes[1,1].title.set_text("Jaro Distance")
        axes[1,2].pie(JW, labels=PieLabels, shadow=True, autopct='%1.2f%%')
        axes[1,2].title.set_text("Jaro Winkler")
        fig.delaxes(ax=axes[1,3])
        plt.show()

    #*Hash
    LD = np.array([ FileData["Levenshtein-Distance Miss Counts"][2], 
                  FileData["IncorrectCount"] - FileData["Levenshtein-Distance Miss Counts"][2]])
    HD = np.array([FileData["Hamming-Distance Miss Counts"][2], 
                  FileData["IncorrectCount"] - FileData["Hamming-Distance Miss Counts"][2]])
    SD = np.array([FileData["Sorensen-Dice Miss Counts"][2],
                  FileData["IncorrectCount"] - FileData["Sorensen-Dice Miss Counts"][2]])
    OS = np.array([FileData["Optimal-String-Alignment Miss Counts"][2],
                  FileData["IncorrectCount"] - FileData["Optimal-String-Alignment Miss Counts"][2]])
    DL = np.array([FileData["Damerau-Levenshtein Miss Counts"][2],
                  FileData["IncorrectCount"] - FileData["Damerau-Levenshtein Miss Counts"][2]])
    JD = np.array([FileData["Jaro-Distance Miss Counts"][2],
                  FileData["IncorrectCount"] - FileData["Jaro-Distance Miss Counts"][2]])
    JW = np.array([FileData["Jaro-Winkler Miss Counts"][2],
                  FileData["IncorrectCount"] - FileData["Jaro-Winkler Miss Counts"][2]])
    

    fig, axes = plt.subplots(2,4)
    plt.suptitle("The ratio of Misses to Non-Misses of spelling %d words with the Hash Search Method" % ((FileData["IncorrectCount"])))

    PieLabels = ["Misses", "Non-Misses"]
    axes[0,0].pie(LD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[0,0].title.set_text("Levenshtein Distance")
    axes[0,1].pie(HD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[0,1].title.set_text("Hamming Distance")
    axes[0,2].pie(SD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[0,2].title.set_text("Sorensen-Dice Distance")
    axes[0,3].pie(OS, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[0,3].title.set_text("Optimal-String-Alignment")
    axes[1,0].pie(DL, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[1,0].title.set_text("Damereau Levenshtein")
    axes[1,1].pie(JD, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[1,1].title.set_text("Jaro Distance")
    axes[1,2].pie(JW, labels=PieLabels, shadow=True, autopct='%1.2f%%')
    axes[1,2].title.set_text("Jaro Winkler")
    fig.delaxes(ax=axes[1,3])
    plt.show()



if __name__ == "__main__":
    if os.path.exists("./CProgramResultsType0.txt") == True:
        C0Data = fileParser("./CProgramResultsType0.txt", 0)
        graphingGenerator(C0Data)
    if os.path.exists("./CProgramResultsType1.txt") == True:
        C1Data = fileParser("./CProgramResultsType1.txt", 1)
        graphingGenerator(C1Data)
    '''
    if os.path.exists("./PythonProgramResultsType0.txt") == True:
        P0Data = fileParser("./PythonProgramResultsType0.txt", 0)
    if os.path.exists("./PythonProgramResultsType1.txt") == True:
        P1Data = fileParser("./PythonProgramResultsType1.txt", 1)
    '''
        
            