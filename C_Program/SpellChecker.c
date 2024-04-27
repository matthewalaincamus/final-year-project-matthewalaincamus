#include "SpellChecker.h"

//skeleton functions for use later
struct WordsListing WordFetcher(int FileType);
struct ReturnHash initHash(int FileType, int MaxSize);
struct TimeAndSuggestion linearCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString[]);
struct TimeAndSuggestion binaryCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString[]);
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[]);
struct TimeAndSuggestion HashChecker(struct WordHashItem *HashTable, int MaxSize, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString[]);
int LevenshteinDistance(char string1[], char string2[], int len1, int len2);
int HammingDistance(char string1[], char string2[], int len1, int len2);
int SorensenDiceCoefficient(char string1[], char string2[], int len1, int len2);
int OptimalStringAlignmentDistance(char string1[], char string2[], int len1, int len2);
int DamerauLevenshteinDistance(char string1[], char string2[], int len1, int len2);
int JaroSimilarity(char string1[], char string2[], int len1, int len2);
int JaroWinklerSimilarity(char string1[], char string2[], int len1, int len2);
int Max(int a, int b);
int Min(int a, int b);
int Min3(int a, int b, int c);
int AutoSpellChecker(int FileType);


//FileType = file used
//CheckType = method used
//algorithmCheck = Algorithm used
int SpellChecker(int FileType, int CheckType, int AlgorithmCheck)
{
    //check to see if the user wants to return to the main menu
    int returnCheck = 1;

    //will loop till the user exits via typing z or Z on the keyboard in the main menu
    while (returnCheck == 1)
    {
        system("cls");

        if (AlgorithmCheck == 0) printf("Simple Check Spell Check:\n");
        else if (AlgorithmCheck == 1) printf("Simple Check Spell Check with Levenshtein Distance based suggestions:\n");
        else if (AlgorithmCheck == 2) printf("Simple Check Spell Check with Hamming Distance based suggestions:\n");
        else if (AlgorithmCheck == 3) printf("Simple Check Spell Check with Sorensen-Dice Coefficient based suggestions:\n");
        else if (AlgorithmCheck == 4) printf("Simple Check Spell Check with Optimal String Alignment Distance based suggestions:\n");
        else if (AlgorithmCheck == 5) printf("Simple Check Spell Check with Damereau-Levenshtein Distance based suggestions:\n");
        else if (AlgorithmCheck == 6) printf("Simple Check Spell Check with Jaro Similarity based suggestions:\n");
        else if (AlgorithmCheck == 7) printf("Simple Check Spell Check with Jaro-Winkler Similarity based suggestions:\n");
        else
        {
            printf("Error: LevCheck value wrong\n");
            return -1;
        }
        printf("**Current Filetype selected: ");

        //display file type info
        if (FileType == 0) printf("*text with duplicate\n");
        else if (FileType == 1) printf("*text without duplicates\n");
        else if (FileType== 2) printf("*text with duplicates sorted in alphabetical order\n");
        else if (FileType == 3) printf("*text without duplicates sorted in alphabetical order\n");

        //display the method used
        if (CheckType == 0) printf("*Method: Word list with linear search\n");
        else if (CheckType == 1) printf("*Method: Word list with binary search\n");
        else if (CheckType == 2) printf("*Method: Word list as a hash table\n");
        else if (CheckType == 3) printf("*Method(s): All");

        printf("\n");

        printf("Please enter the word you wish to spell check\n");
        printf("RULES:\n");
        printf("- no numbers or non alphabet characters\n");
        printf("- maximum length string of 50 characters\n");
        printf("- all characters must be in lowercase\n\n");
        printf("If you wish to return to the previous screen, please type '?' and then press enter\n");

        int inputCheck = 1;

        char ActionChoice[128];

        while(inputCheck == 1)
        {
            //user input
            printf(": ");
            scanf("%s", ActionChoice);

            //question mark denotes return to menu
            if(!strcmp(ActionChoice, "?")) return 0;

            int WordCheck = 0;

            for (int i = 0; i < 128; i++)
            {
                if (i > 49) WordCheck++;

                int charToInt = (int)ActionChoice[i];

                if (ActionChoice[i] == '\0') break;
                else if (isdigit(ActionChoice[i])) WordCheck++;
                else if (charToInt < 97 || charToInt > 122) WordCheck++;
            }

            if (WordCheck > 0) printf("Please enter a valid word\n");
            else inputCheck = 0;
        }

        if (CheckType == 0 || CheckType == 1)
        {
            struct WordsListing WordArray = WordFetcher(FileType);
            if (WordArray.WordCount == -1) return -1;

            //start timer
            struct timeb Start;
            ftime(&Start);

            //linear searching = 0
            if (CheckType == 0) linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, AlgorithmCheck, 0, "");

            //binary searching = 1
            else if (CheckType == 1) binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, AlgorithmCheck, 0, ""); 
        
            //cleanup
            free(WordArray.WordArray);
            WordArray.WordArray = NULL;
        }
        else if (CheckType == 2)
        {
            //max size of hash table
            int MaxSize = 10000;

            //create the hash table from the list of words
            struct ReturnHash WordHashTable = initHash(FileType, MaxSize);
            if (WordHashTable.WordCount == -1) return -1;

            /*
            struct WordNode *TestNode;
            for (int i = 0; i < MaxSize; i++)
            {
                if (WordHashTable.HashTable[i].head != NULL && i < 6000 && i >5000)
                {
                    printf("(%d)Head: ", i);
                    TestNode = WordHashTable.HashTable[i].head;
                    if (TestNode->next == NULL) printf("(%d : %s) :Tail",TestNode->key,TestNode->Word);
                    else
                    {
                        while(1)
                        {   
                            if (TestNode->next == NULL)
                            {
                                printf(" :Tail");
                                break;
                            }

                            printf("(%d : %s) -> ", TestNode->key,TestNode->Word);
                            TestNode = TestNode->next;
                        }
                    }
                    printf("\n");
                }
            }
            */

            //perform hash checking
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, AlgorithmCheck, 0, "");

            //cleanup
            struct WordNode *tmp, *currentNode;
            for (int i = 0; i < MaxSize; i++)
            {
                if (WordHashTable.HashTable[i].head != NULL && WordHashTable.HashTable[i].tail != NULL)
                {
                    currentNode = WordHashTable.HashTable[i].head;
                    while(currentNode->next != NULL)
                    {
                        tmp = currentNode->next;
                        free(currentNode);
                        currentNode = tmp;
                    }
                    tmp = currentNode;
                    free(tmp);
                }
            }
        }
        printf("\n");

        //user input (just to confirm that the user can return back to the previous menu)
        char ActionChoice2[128];
        printf("Type anything to enter another word: ");
        scanf("%s", ActionChoice2);
    }

    return 0;
}

//convert word file into a single word array
struct WordsListing WordFetcher(int FileType)
{
    struct WordsListing returnStruct;

    FILE* fp;

    if (FileType == 0)
    {
        fp = fopen("./GeneratedFiles/LWD.txt", "r");
    }
    else if(FileType == 1)
    {
        fp = fopen("./GeneratedFiles/LWND.txt", "r");
    }
    else if(FileType == 2)
    {
        fp = fopen("./GeneratedFiles/LWDS.txt", "r");
    }
    else if(FileType == 3)
    {
        fp = fopen("./GeneratedFiles/LWNDS.txt", "r");
    }
    else
    {
        printf("Error: bad fileflag found\n");
        returnStruct.WordCount = -1;
        return returnStruct;
    }

    if (fp == NULL)
    {
        printf("Error: unable to open word file\n");
        returnStruct.WordCount = -1;
        return returnStruct;
    }

    //word array for storing all the words
    char **WordArray;
    WordArray = malloc(20000000 * sizeof(char*));
    int WordCount = 0;

    char nextChar = ' ';
    char nextString[50];

    //for testing
    int lineCount = 0;
    
    int charCount = 0;

    while (nextChar != EOF)
    {
        nextChar = fgetc(fp);

        if (isalpha(nextChar))
        {
            nextString[charCount] = nextChar;
            charCount++;
        }
        else if (nextChar == '\n')
        {
            nextString[charCount] = '\0';
            //printf("%s\n", nextString);

            WordArray[WordCount] = malloc(50 * sizeof(char));
            strcpy(WordArray[WordCount], nextString);
            WordCount++;
            
            lineCount++;
            charCount = 0;
        }
    }

    fclose(fp);

    returnStruct.WordArray = WordArray;
    returnStruct.WordCount = WordCount;

    return returnStruct;
}

struct ReturnHash initHash(int FileType, int MaxSize)
{
    FILE* fp;

    struct ReturnHash HashToReturn;
    struct WordHashItem *returnHashTable = (struct WordHashItem*) malloc(MaxSize * sizeof(struct WordHashItem));

    //initialise the array
    for (int i = 0; i < MaxSize; i++)
    {
        returnHashTable[i].head = NULL;
        returnHashTable[i].tail = NULL;
    }

    //open the right file
    if (FileType == 0)
    {
        fp = fopen("./GeneratedFiles/LWD.txt", "r");
    }
    else if(FileType == 1)
    {
        fp = fopen("./GeneratedFiles/LWND.txt", "r");
    }
    else if(FileType == 2)
    {
        fp = fopen("./GeneratedFiles/LWDS.txt", "r");
    }
    else if(FileType == 3)
    {
        fp = fopen("./GeneratedFiles/LWNDS.txt", "r");
    }
    else
    {
        printf("Error: bad fileflag found\n");
        HashToReturn.HashTable = returnHashTable;
        HashToReturn.WordCount = -1;
        return HashToReturn;
    }

    if (fp == NULL)
    {
        printf("Error: unable to open word file\n");
        HashToReturn.HashTable = returnHashTable;
        HashToReturn.WordCount = -1;
        return HashToReturn;
    }

    int WordCount = 0;

    char nextChar = ' ';
    char nextString[50];

    //for testing
    int lineCount = 0;
    
    int charCount = 0;

    int stringHashCode = 1;

    while (nextChar != EOF)
    {
        nextChar = fgetc(fp);

        if (isalpha(nextChar))
        {
            nextString[charCount] = nextChar;
            charCount++;
            //add ascii value of char to hash code
            stringHashCode = (stringHashCode * (int)nextChar) % MaxSize;
            //so most of the nodes don't congregate at zero
            if (stringHashCode == 0) stringHashCode++;
        }
        else if (nextChar == '\n')
        {
            //end off the string
            nextString[charCount] = '\0';

            //testing
            //if (!strcmp(nextString, "apple")) printf("%s : %d\n", nextString, stringHashCode);
            
            //insert the string into the hash table
            struct WordNode *nodeList = (struct WordNode*) returnHashTable[stringHashCode].head;

            //create the item to add to the hash table 
            struct WordNode *WordItem = (struct WordNode*)malloc(sizeof(struct WordNode));
            strcpy(WordItem->Word, nextString);
            WordItem->next = NULL;
            WordItem->key = 0;
            WordItem->AlgorithmDistance = 100;

            //if there are no elements at the index
            if (nodeList == NULL)
            {
                returnHashTable[stringHashCode].head = WordItem;
                returnHashTable[stringHashCode].tail = WordItem;
            }
            
            //if there is a linked list already there
            else
            {
                //update the tail to the new item
                WordItem->key = returnHashTable[stringHashCode].tail->key + 1;
                returnHashTable[stringHashCode].tail->next = WordItem;
                returnHashTable[stringHashCode].tail = WordItem;
            }
            

            //update the counts ready for next word
            WordCount++;
            lineCount++;
            charCount = 0;
            stringHashCode = 1;
        }
    }

    fclose(fp);

    HashToReturn.HashTable = returnHashTable;
    HashToReturn.WordCount = WordCount;
    return HashToReturn;
}

//linear search checker
struct TimeAndSuggestion linearCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString[])
{
    //start timer:
    struct timeb Start;
    ftime(&Start);

    //return values (only useful for auto tester)
    struct TimeAndSuggestion returnValues;
    returnValues.SuggestionNumber = 0;

    //if called from automated testing, print out data differently
    if (TestFlag == 1)
    {
        //first print out algorithm used:
        if (AlgorithmCheck == 0) printf("|%-30s", "None");
        else if (AlgorithmCheck == 1) printf("|%-30s", "LevenshteinDistance");
        else if (AlgorithmCheck == 2) printf("|%-30s", "HammingDistance");
        else if (AlgorithmCheck == 3) printf("|%-30s", "SorensenDiceCoefficient");
        else if (AlgorithmCheck == 4) printf("|%-30s", "OptimalStringAlignmentDistance");
        else if (AlgorithmCheck == 5) printf("|%-30s", "DamerauLevenshteinDistance");
        else if (AlgorithmCheck == 6) printf("|%-30s", "JaroSimilarity");
        else if (AlgorithmCheck == 7) printf("|%-30s", "JaroWinklerSimilarity");
    }
    int WordFlag = 0;
    int lineCount = 0;

    //for suggestion calculations
    int AlgorithmArray [WordCount];

    //if the word is a single letter, no need to do any further checks as they are always valid
    if (strlen(wordString) == 1)
    {
        if (TestFlag == 0) printf("Your word exists in the word list\n");
        WordFlag++;
    }
    else
    {
        for (int i = 0; i < WordCount; i++)
        {
            if(!strcmp(WordArray[i], wordString))
            {
                if (TestFlag == 0) printf("Your word exists in the word list\n");
                WordFlag++;
                break;
            }
            lineCount++;
            
            //if an algorithm is active, get the distance between the two words
            if (AlgorithmCheck == 1) AlgorithmArray[i] = LevenshteinDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            else if (AlgorithmCheck == 2) AlgorithmArray[i] = HammingDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            else if (AlgorithmCheck == 3) AlgorithmArray[i] = SorensenDiceCoefficient(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            else if (AlgorithmCheck == 4) AlgorithmArray[i] = OptimalStringAlignmentDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            else if (AlgorithmCheck == 5) AlgorithmArray[i] = DamerauLevenshteinDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            else if (AlgorithmCheck == 6) AlgorithmArray[i] = JaroSimilarity(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            else if (AlgorithmCheck == 7) AlgorithmArray[i] = JaroWinklerSimilarity(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
        }
    }

    //if word wasn't found, get suggestions
    if (WordFlag == 0)
    {
        if (TestFlag == 0) printf("Your word doesn't exist in the word list\n");
        else if (TestFlag == 1) printf("|%-15s", "False");

        //if anything other than a simple check, get the top 10 results of the specified algorithm
        if (AlgorithmCheck != 0)
        {
            //get top 10 suggestions
            for (int i = 0; i < 10; i++)
            {
                int CurrentMin = 100;
                int CurrentMinIndex = 0;

                for(int j = 0; j < WordCount; j++)
                {
                    if (AlgorithmArray[j] < CurrentMin)
                    {
                        CurrentMin = AlgorithmArray[j];
                        CurrentMinIndex = j;
                    }
                }

                AlgorithmArray[CurrentMinIndex] = 100;

                if (TestFlag == 0) printf("Suggestion %i: %s\n", i+1, WordArray[CurrentMinIndex]);
                else if (TestFlag == 1 && i == 0) printf("|%-25s|\n", WordArray[CurrentMinIndex]);
                else if (TestFlag == 1 && i > 0) printf("|%-30s|%-15s|%-25s|\n", "", "",  WordArray[CurrentMinIndex]);
                else if (TestFlag == 2 && !strcmp(WordArray[CurrentMinIndex], TestString)) returnValues.SuggestionNumber = i+1;

            }
        }
        else if (AlgorithmCheck == 0 && TestFlag == 1) printf("|%-25s|\n", "N/A");
    }
    else if (WordFlag > 0 && TestFlag == 1) printf("|%-15s|%-25s|\n", "True", "");

    //get the time of completion
    struct timeb Stop;
    ftime(&Stop);

    float TimeTaken = 1000.0 * (Stop.time - Start.time) + (Stop.millitm - Start.millitm);

    if (TestFlag == 0) printf("Operation took: %f miliseconds to complete\n\n", TimeTaken);
    else if (TestFlag == 1) printf("|Time to complete: %-10f ms%-41s|\n",  TimeTaken, "");
    else if (TestFlag == 2) returnValues.TimeTaken = TimeTaken;
    
    return returnValues;
}

//binary search checker
struct TimeAndSuggestion binaryCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString[])
{
    //start timer:
    struct timeb Start;
    ftime(&Start);

    //return values (only useful for auto tester)
    struct TimeAndSuggestion returnValues;
    returnValues.SuggestionNumber = 0;

    //if called from automated testing, print out data differently
    if (TestFlag == 1)
    {
        //first print out algorithm used:
        if (AlgorithmCheck == 0) printf("|%-30s", "None");
        else if (AlgorithmCheck == 1) printf("|%-30s", "LevenshteinDistance");
        else if (AlgorithmCheck == 2) printf("|%-30s", "HammingDistance");
        else if (AlgorithmCheck == 3) printf("|%-30s", "SorensenDiceCoefficient");
        else if (AlgorithmCheck == 4) printf("|%-30s", "OptimalStringAlignmentDistance");
        else if (AlgorithmCheck == 5) printf("|%-30s", "DamerauLevenshteinDistance");
        else if (AlgorithmCheck == 6) printf("|%-30s", "JaroSimilarity");
        else if (AlgorithmCheck == 7) printf("|%-30s", "JaroWinklerSimilarity");
    }

    //perform the binary search
    int binaryCheck = binarySearch(WordArray, 0, WordCount - 1, wordString);
    if (binaryCheck == 0) 
    {
        if (TestFlag == 0) printf("Your word doesn't exist in the word list\n");
        else if (TestFlag == 1) printf("|%-15s", "False");

        if (AlgorithmCheck != 0)
        {
            //for suggestion calculations
            int AlgorithmArray [WordCount];

            //get all the distances from the list linearly
            for (int i = 0; i < WordCount; i++)
            {   
                if (AlgorithmCheck == 1) AlgorithmArray[i] = LevenshteinDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                else if(AlgorithmCheck == 2) AlgorithmArray[i] = HammingDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                else if (AlgorithmCheck == 3) AlgorithmArray[i] = SorensenDiceCoefficient(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                else if (AlgorithmCheck == 4) AlgorithmArray[i] = OptimalStringAlignmentDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                else if (AlgorithmCheck == 5) AlgorithmArray[i] = DamerauLevenshteinDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                else if (AlgorithmCheck == 6) AlgorithmArray[i] = JaroSimilarity(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                else if (AlgorithmCheck == 7) AlgorithmArray[i] = JaroWinklerSimilarity(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            }

            //if anything other than a simple check, get the top 10 results of the specified algorithm
            if (AlgorithmCheck != 0)
            {
                //get top 10 suggestions
                for (int i = 0; i < 10; i++)
                {
                    int CurrentMin = 100;
                    int CurrentMinIndex = 0;

                    for(int j = 0; j < WordCount; j++)
                    {
                        if (AlgorithmArray[j] < CurrentMin)
                        {
                            CurrentMin = AlgorithmArray[j];
                            CurrentMinIndex = j;
                        }
                    }

                    AlgorithmArray[CurrentMinIndex] = 100;

                    if (TestFlag == 0) printf("Suggestion %i: %s\n", i+1, WordArray[CurrentMinIndex]);
                    else if (TestFlag == 1 && i == 0) printf("|%-25s|\n", WordArray[CurrentMinIndex]);
                    else if (TestFlag == 1 && i > 0) printf("|%-30s|%-15s|%-25s|\n", "", "",  WordArray[CurrentMinIndex]);
                    else if (TestFlag == 2 && !strcmp(WordArray[CurrentMinIndex], TestString)) returnValues.SuggestionNumber = i+1;
                }
            }
        }
        else if (AlgorithmCheck == 0 && TestFlag == 1) printf("|%-25s|\n", "N/A");
    }
    else if (binaryCheck == 1) 
    {
        if (TestFlag == 0) printf("Your word exists in the word list\n");
        else if (TestFlag == 1) printf("|%-15s|%-25s|\n", "True", "");
    }
    else printf("Error: problem with binary search\n");

    //get the time taken
    struct timeb Stop;
    ftime(&Stop);

    float TimeTaken = 1000.0 * (Stop.time - Start.time) + (Stop.millitm - Start.millitm);

    if (TestFlag == 0) printf("Operation took: %f miliseconds to complete\n\n", TimeTaken);
    else if (TestFlag == 1) printf("|Time to complete: %-10f ms%-41s|\n",  TimeTaken, "");
    else if (TestFlag == 2) returnValues.TimeTaken = TimeTaken;
    
    return returnValues;
}

//hash table checker
struct TimeAndSuggestion HashChecker(struct WordHashItem *HashTable, int MaxSize, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString[])
{
    //start timer:
    struct timeb Start;
    ftime(&Start);

    //return values (only useful for auto tester)
    struct TimeAndSuggestion returnValues;
    returnValues.SuggestionNumber = 0;

    //if called from automated testing, print out data differently
    if (TestFlag == 1)
    {
        //first print out algorithm used:
        if (AlgorithmCheck == 0) printf("|%-30s", "None");
        else if (AlgorithmCheck == 1) printf("|%-30s", "LevenshteinDistance");
        else if (AlgorithmCheck == 2) printf("|%-30s", "HammingDistance");
        else if (AlgorithmCheck == 3) printf("|%-30s", "SorensenDiceCoefficient");
        else if (AlgorithmCheck == 4) printf("|%-30s", "OptimalStringAlignmentDistance");
        else if (AlgorithmCheck == 5) printf("|%-30s", "DamerauLevenshteinDistance");
        else if (AlgorithmCheck == 6) printf("|%-30s", "JaroSimilarity");
        else if (AlgorithmCheck == 7) printf("|%-30s", "JaroWinklerSimilarity");
    }

    //for showing if a word has been found or not
    int WordFlag = 0;

    //if the word is a single letter, no need to do any further checks as they are always valid
    if (strlen(wordString) == 1)
    {
        if (TestFlag == 0) printf("Your word exists in the word list\n");
        WordFlag++;
    }
    else
    {
        //get the hash value for the given string
        int StringHash = 1;
        for (int i = 0; i < strlen(wordString); i++)
        {
            StringHash = (StringHash * (int)wordString[i]) % MaxSize;
            if (StringHash == 0) StringHash++;
        }

        //get the linked list from the index
        //first check if the word is at the head
        struct WordNode *LinkedList = HashTable[StringHash].head;

        //provided there is anything there at the index
        if (LinkedList != NULL)
        {
            if (!strcmp(LinkedList->Word, wordString))
            {
                if (TestFlag == 0) printf("Your word exists in the word list\n");
                WordFlag++;
            }
            //if not, loop through the linked list till you find the word or reach the tail
            else
            {
                while (LinkedList->next != NULL)
                {
                    LinkedList = LinkedList->next;
                    if (!strcmp(LinkedList->Word, wordString))
                    {
                        if (TestFlag == 0) printf("Your word exists in the word list\n");
                        WordFlag++;
                        break;
                    }
                }
            }
        }
    }

    //if word wasn't found, get suggestions
    if (WordFlag == 0)
    {
        if (TestFlag == 0) printf("Your word doesn't exist in the word list\n");
        else if (TestFlag == 1) printf("|%-15s", "False");
        
        if (AlgorithmCheck != 0)
        {
            for (int i = 0; i < MaxSize; i++)
            {
                if (HashTable[i].head != NULL && HashTable[i].tail != NULL)
                {
                    struct WordNode *currentNode = HashTable[i].head;

                    //in the case that the linked list at the point only has 1 node
                    if (currentNode->next == NULL)
                    {
                        //if an algorithm is active, get the distance between the two words
                        if (AlgorithmCheck == 1) currentNode->AlgorithmDistance = LevenshteinDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                        else if (AlgorithmCheck == 2) currentNode->AlgorithmDistance = HammingDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                        else if (AlgorithmCheck == 3) currentNode->AlgorithmDistance = SorensenDiceCoefficient(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                        else if (AlgorithmCheck == 4) currentNode->AlgorithmDistance = OptimalStringAlignmentDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                        else if (AlgorithmCheck == 5) currentNode->AlgorithmDistance = DamerauLevenshteinDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                        else if (AlgorithmCheck == 6) currentNode->AlgorithmDistance = JaroSimilarity(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                        else if (AlgorithmCheck == 7) currentNode->AlgorithmDistance = JaroWinklerSimilarity(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                    }

                    //if there are more than 1 nodes
                    else
                    {   //if an algorithm is active, get the distance between the two words
                            if (AlgorithmCheck == 1) currentNode->AlgorithmDistance = LevenshteinDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 2) currentNode->AlgorithmDistance = HammingDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 3) currentNode->AlgorithmDistance = SorensenDiceCoefficient(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 4) currentNode->AlgorithmDistance = OptimalStringAlignmentDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 5) currentNode->AlgorithmDistance = DamerauLevenshteinDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 6) currentNode->AlgorithmDistance = JaroSimilarity(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 7) currentNode->AlgorithmDistance = JaroWinklerSimilarity(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                        while(currentNode->next != NULL)
                        {
                            currentNode = currentNode->next;
                            //if an algorithm is active, get the distance between the two words
                            if (AlgorithmCheck == 1) currentNode->AlgorithmDistance = LevenshteinDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 2) currentNode->AlgorithmDistance = HammingDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 3) currentNode->AlgorithmDistance = SorensenDiceCoefficient(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 4) currentNode->AlgorithmDistance = OptimalStringAlignmentDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 5) currentNode->AlgorithmDistance = DamerauLevenshteinDistance(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 6) currentNode->AlgorithmDistance = JaroSimilarity(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                            else if (AlgorithmCheck == 7) currentNode->AlgorithmDistance = JaroWinklerSimilarity(wordString, currentNode->Word, strlen(wordString), strlen(currentNode->Word));
                        }
                    }
                }
            }

            //if anything other than a simple check, get the top 10 results of the specified algorithm
            for (int i = 0; i < 10; i++)
            {
                int CurrentMin = 100;
                int CurrentMinIndex = 0;
                int CurrentMinLinkedListIndex = 0;

                //loop through the entire hash table
                for (int j = 0; j < MaxSize; j++)
                {
                    //if there is a value the certain index
                    if (HashTable[j].head != NULL && HashTable[j].tail != NULL)
                    {
                        struct WordNode *currentNode = HashTable[j].head;
                        if (currentNode->AlgorithmDistance < CurrentMin)
                        {
                            CurrentMin = currentNode->AlgorithmDistance;
                            CurrentMinIndex = j;
                            CurrentMinLinkedListIndex = currentNode->key;
                        }
                                                
                        while(currentNode->next != NULL)
                        {
                            currentNode = currentNode->next;
                            if (currentNode->AlgorithmDistance < CurrentMin)
                            {
                                CurrentMin = currentNode->AlgorithmDistance;
                                CurrentMinIndex = j;
                                CurrentMinLinkedListIndex = currentNode->key;
                            }
                        }
                        
                    }
                }

                /*
                if (i == 0)
                {
                    struct WordNode *TestNode;
                    for (int k = 0; k < MaxSize; k++)
                    {
                        if (HashTable[k].head != NULL && HashTable[k].tail != NULL && k < 8000)
                        {
                            printf("%d| ", k);
                            TestNode = HashTable[k].head;
                            if (TestNode->next == NULL) printf("%d:%d: %s",TestNode->key, TestNode->AlgorithmDistance, TestNode->Word);
                            while(TestNode->next != NULL)
                            {   
                                printf("%d:%d: %s ->", TestNode->key, TestNode->AlgorithmDistance, TestNode->Word);
                                TestNode = TestNode->next;
                            }
                            printf("\n");
                        }
                    }
                }
                */

                //update the min key to be large and out of the way
                struct WordNode *updateNode = HashTable[CurrentMinIndex].head;
                for (int k = 0; k < CurrentMinLinkedListIndex; k++) 
                {
                    updateNode = updateNode->next;
                }
                updateNode->AlgorithmDistance = 100;

                if (TestFlag == 0) printf("Suggestion %i: %s\n", i+1, updateNode->Word);
                else if (TestFlag == 1 && i == 0) printf("|%-25s|\n", updateNode->Word);
                else if (TestFlag == 1 && i > 0) printf("|%-30s|%-15s|%-25s|\n", "", "",  updateNode->Word);
                else if (TestFlag == 2 && !strcmp(updateNode->Word, TestString)) returnValues.SuggestionNumber = i+1;
            }
        }
        else if (AlgorithmCheck == 0 && TestFlag == 1) printf("|%-25s|\n", "N/A");
    }
    else if (WordFlag > 0 && TestFlag == 1) printf("|%-15s|%-25s|\n", "True", "");

    //get the time of completion
    struct timeb Stop;
    ftime(&Stop);

    float TimeTaken = 1000.0 * (Stop.time - Start.time) + (Stop.millitm - Start.millitm);

    if (TestFlag == 0) printf("Operation took: %f miliseconds to complete\n\n", TimeTaken);
    else if (TestFlag == 1) printf("|Time to complete: %-10f ms%-41s|\n",  TimeTaken, "");
    else if (TestFlag == 2) returnValues.TimeTaken = TimeTaken;

    return returnValues;
}

//binary search recursive algorithm
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[])
{
    if (rightValue >= leftValue)
    {
        int middleValue = leftValue + (rightValue - leftValue) / 2;

        if (strcmp(wordString, WordArray[middleValue]) == 0)
        return 1;

        else if (strcmp(wordString, WordArray[middleValue]) < 0)
        {
            return binarySearch(WordArray, leftValue, middleValue - 1, wordString);
        }
        else
        {
            return binarySearch(WordArray, middleValue + 1, rightValue, wordString);
        }
    }
    return 0;
}

//Levenshtein distance calculator
int LevenshteinDistance(char string1[], char string2[], int len1, int len2)
{
    //array to store results of calculations
    int CalculationArray[len1+1][len2+1];

    //add starting values to calculation array
    for (int i = 0; i <= len1; i++) CalculationArray[i][0] = i;
    for (int j = 0; j <= len2; j++) CalculationArray[0][j] = j;

    //fill in the matrix using dynamic programming
    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            if (string1[i - 1] == string2[j - 1])
            {
                //characters match, no operation needed
                CalculationArray[i][j] = CalculationArray[i-1][j-1];
            }
            else
            {
                //characters don't match, choose minimum cost among insertion, deletion, substitution
                CalculationArray[i][j] = 1 + Min3(
                                        CalculationArray[i][j-1],
                                        CalculationArray[i-1][j],
                                        CalculationArray[i-1][j-1]);
            }
        }
    }
    return CalculationArray[len1][len2];
}

//hamming distance calculator
int HammingDistance(char string1[], char string2[], int len1, int len2)
{
    //strings need to be same length
    if (len1 != len2) return 100; 

    int HammingScore = 0;

    //if a char is different, add 1 to the score
    for (int i = 0; i < len1; i++)
    {
        if (string1[i] != string2[i]) HammingScore++;
    }

    return HammingScore;
}

//hamming distance calculator
int SorensenDiceCoefficient(char string1[], char string2[], int len1, int len2)
{
    //number of bigrams that match
    double matches = 0;

    int i = 0, j = 0;

    //loop through, getting the number of bigrams that match and updating the count
    while (i < len1 - 1 && j < len2 - 1)
    {
        char bigram1[3] = {string1[i], string2[i+1], '\0'};
        char bigram2[3] = {string2[j], string2[j+1], '\0'};

        if (!strcmp(bigram1, bigram2)) matches++;

        ++i;
        ++j;
    }

    //would usually be a value between 0 and 1, but for uniformity of keeping all algorithm values
    // int, times the value by 100 and use that instead
    //also, minus the value from 100, so that similar values are close to 0 than 100 so the 
    // top 10 results calculator and just look for the smallest values
    return 100 - (100 * ( (2 * matches) / ( (len1 - 1) + (len2 - 1) ) ) );
}

//Optimal String Alignment Distance calculator
int OptimalStringAlignmentDistance(char string1[], char string2[], int len1, int len2)
{
    //array to store results of calculations
    int CalculationArray[len1+1][len2+1];

    //add starting values to calculation array
    for (int i = 0; i <= len1; i++) CalculationArray[i][0] = i;
    for (int j = 0; j <= len2; j++) CalculationArray[0][j] = j;

    for (int i = 0; i <= len1; i++)
    {
        for (int j = 0; j <= len2; j++)
        {
            int cost;

            if (string1[i] == string2[j]) cost = 0;
            else cost = 1;

            CalculationArray[i+1][j+1] = Min3(CalculationArray[i][j+1] + 1, //deletion
                                            CalculationArray[i+1][j] + 1, //insertion
                                            CalculationArray[i][j] + cost); //substitution
            
            if ((i > 0) && (j > 0) && (string1[i+1] == string2[j]) && (string1[i] == string2[j+1]))
            {
                CalculationArray[i][j] = Min(CalculationArray[i][j],
                                                CalculationArray[i-1][j-1] + 1); //transposition
            }
        }   
    }

    return CalculationArray[len1][len2];
}

//Damerau-Levenshtein distance calculator
int DamerauLevenshteinDistance(char string1[], char string2[], int len1, int len2)
{
    
    //create the 2D array for storing calculations
    int CalculationArray[len1+2][len2+2];

    //max distance for transpositions 
    int MaxDist = len1 + len2;
    
    CalculationArray[0][0] = MaxDist;
    for (int i = 0; i <= len1; i++)
    {
        CalculationArray[i+1][1] = i;
        CalculationArray[i+1][0] = MaxDist;
    }
    for (int j = 0; j <= len2; j++)
    {
        CalculationArray[1][j+1] = j;
        CalculationArray[0][j+1] = MaxDist;
    }

    //store the row where a given character last appeared in string1
    int lastRow[256];
    
    for (int d = 0; d < 256; d++)
        lastRow[d] = 0;

    // calculation loop
    for (int i = 1; i <= len1; i++)
    {
        //convert to unsigned so that they can be used as an index to last_row
        unsigned char string1Char = string1[i-1];
        
        // The last column this row where the character in string1 matched the character in string2
        int lastMatchingCol = 0;

        for (int j = 1; j <= len2; j++)
        {
            //convert to unsigned so that they can be used as an index to last_row
            unsigned char string2Char = string2[j-1];
            
            // The last place in string1 where we can find the current character in string2
            int lastMatchingRow = lastRow[string2Char];

            int cost;
            if (string1Char == string2Char) cost = 0;
            else cost = 1;

            CalculationArray[i+1][j+1] = Min( Min(CalculationArray[i][j+1] + 1, //insertion
                                               CalculationArray[i+1][j] + 1), //deletion
                                               Min(CalculationArray[i][j] + cost, //substitution
                                               CalculationArray[lastMatchingRow][lastMatchingCol]
                                                + (i - lastMatchingRow - 1) + 1
                                                + (j - lastMatchingCol - 1) ) ); //transposition


            // If there was a match, update the last matching column
            if (cost == 0)
                lastMatchingCol = j;
        }

        // Update the entry for this row's character
        lastRow[string1Char] = i;
    }

    // Extract the bottom right-most cell -- that's the total distance
    int result = CalculationArray[len1+1][len2+1];

    return result;
}

//Jaro Similarity calculator
int JaroSimilarity(char string1[], char string2[], int len1, int len2)
{
    //max distance for which matching is allowed
    int maxDist = floor(Max(len1, len2) / 2) - 1;

    //count for number of total matches
    int matches = 0;

    //hashes to store matches
    int String1Matches[len1];
    int String2Matches[len2];

    for (int i = 0; i < len1; i++)String1Matches[i] = 0;
    for (int j = 0; j < len2; j++)String2Matches[j] = 0;

    //traverse the first string
    for (int i = 0; i < len1; i++)
    {
        //check if there are any matches
        for (int j = Max(0, i - maxDist); j < Min(len2, i + maxDist + 1); j++)
        {
            if ((string1[i] == string2[j]) && (String2Matches[j] == 0))
            {
                String1Matches[i] = 1;
                String2Matches[j] = 1;
                matches++;
                break;
            } 
        }
    }

    //if nothing matches, return the larges value possible
    if (matches == 0) return 100;

    //number of transpositions
    double t = 0;

    int point = 0;

    for(int i = 0; i < len1; i++)
    {
        if (String1Matches[i])
        {
            while (String2Matches[point] == 0) point++;

            if (string1[i] != string2[point++]) t++;
        }
    }

    t /= 2;

    // Jaro Similarity Returned
    //times by 100 and then subtract from 100 so that similar values are converted to int and close to zero
    return 100 - (100 * ( ( ( (double)matches ) / ( (double)len1 ) 
                        + ( (double)matches) / ( (double)len2 ) 
                        + ( (double)matches - t) / ( (double) matches) ) / 3.0));
}

//Jaro-Winkler Similarity calculator
int JaroWinklerSimilarity(char string1[], char string2[], int len1, int len2)
{
    //get the original jaro distance
    double jaroDistance = (double)(100 - JaroSimilarity(string1, string2, len1, len2)) / 100;

    //if the similarity is above a threshold
    if (jaroDistance > 0.7)
    {
        int Prefix = 0;

        for (int i = 0; i < Min(len1, len2); i++)
        {
            //if the characters match
            if (string1[i] == string2[i]) Prefix++;
            else break;
        }

        //maximum of 4 characters are allowed in prefix
        Prefix = Min(4, Prefix);

        //calculate jaro winkler Similarity
        jaroDistance += 0.1 * Prefix * (1 - jaroDistance);
    }

    //return jaro distance, making sure values are int and similar values are close to zero
    return 100 - (100 * jaroDistance);
}

//getting the maximum of 2 numbers
int Max(int a, int b)
{
    if (a > b) return a;
    else return b;
}

//getting the minimum of 2 numbers
int Min(int a, int b)
{
    if (a < b) return a;
    else return b;
}

//getting the minimum of 3 numbers
int Min3(int a, int b, int c)
{
    if ((a < b) && (a < c)) 
    {
        return a;
    }
    else if (b < c) return b;
    else return c;
}

int AutoSpellChecker(int FileType)
{
    //check to see if the user wants to return to the main menu
    int returnCheck = 1;

    //will loop till the user exits via typing z or Z on the keyboard in the main menu
    while (returnCheck == 1)
    {
        system("cls");

        printf("Full Spell Check:\n");
        
        printf("**Current Filetype selected: ");

        //display file type info
        if (FileType == 0) printf("*text with duplicate\n");
        else if (FileType == 1) printf("*text without duplicates\n");
        else if (FileType== 2) printf("*text with duplicates sorted in alphabetical order\n");
        else if (FileType == 3) printf("*text without duplicates sorted in alphabetical order\n");

        printf("\n");

        printf("Please enter the word you wish to spell check\n");
        printf("RULES:\n");
        printf("- no numbers or non alphabet characters\n");
        printf("- maximum length string of 50 characters\n");
        printf("- all characters must be in lowercase\n\n");
        printf("If you wish to return to the previous screen, please type '?'\n");
        printf("If you wish to launch the mispelled word tester, please type '!'\n");

        int inputCheck = 1;

        char ActionChoice[128];

        while(inputCheck == 1)
        {
            //user input
            printf(": ");
            scanf("%s", ActionChoice);

            //question mark denotes return to menu
            if(!strcmp(ActionChoice, "?")) return 0;
            if(!strcmp(ActionChoice, "!")) break;

            int WordCheck = 0;

            for (int i = 0; i < 128; i++)
            {
                if (i > 49) WordCheck++;

                int charToInt = (int)ActionChoice[i];

                if (ActionChoice[i] == '\0') break;
                else if (isdigit(ActionChoice[i])) WordCheck++;
                else if (charToInt < 97 || charToInt > 122) WordCheck++;
            }

            if (WordCheck > 0) printf("Please enter a valid word\n");
            else inputCheck = 0;
        }

        if (!strcmp(ActionChoice,"!"))
        {
            AlgorithmAssessor(FileType);
        }
        else
        {

            //start the process of testing every method and algorithm all together

            //first generate the wordlist data structure for linear and binary searching
            struct WordsListing WordArray = WordFetcher(FileType);
            if (WordArray.WordCount == -1) return -1;

            //then begin testing:

            //**linear:
            printf("**Method: Linear Search**\n");
            printf("--------------------------------------------------------------------------\n");
            printf("|Algorithm:                    |StringFound:   |Suggestions:             |\n");
            printf("|------------------------------+---------------+-------------------------|\n");

            //no algorithm
            linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 0, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Levenshtein Distance
            linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 1, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Hamming Distance
            linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 2, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Sørensen–Dice Coefficient
            linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 3, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Optimal String Alignment Distance
            linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 4, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Damerau-Levenshtein Distance
            linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 5, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Jaro Distance
            linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 6, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Jaro-Winkler Distance
            linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 7, 1, "");

            printf("--------------------------------------------------------------------------\n");
            printf("\n\n");

            //**binary:
            printf("**Method: Binary Search**\n");

            //need a sorted array to use this
            if (FileType == 3)
            {
                printf("--------------------------------------------------------------------------\n");
                printf("|Algorithm:                    |StringFound:   |Suggestions:             |\n");
                printf("|------------------------------+---------------+-------------------------|\n");

                //no algorithm
                binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 0, 1, "");
                printf("|------------------------------+---------------+-------------------------|\n");

                //Levenshtein Distance
                binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 1, 1, "");
                printf("|------------------------------+---------------+-------------------------|\n");

                //Hamming Distance
                binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 2, 1, "");
                printf("|------------------------------+---------------+-------------------------|\n");

                //Sørensen–Dice Coefficient
                binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 3, 1, "");
                printf("|------------------------------+---------------+-------------------------|\n");

                //Optimal String Alignment Distance
                binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 4, 1, "");
                printf("|------------------------------+---------------+-------------------------|\n");

                //Damerau-Levenshtein Distance
                binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 5, 1, "");
                printf("|------------------------------+---------------+-------------------------|\n");

                //Jaro Distance
                binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 6, 1, "");
                printf("|------------------------------+---------------+-------------------------|\n");

                //Jaro-Winkler Distance
                binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 7, 1, "");

                printf("--------------------------------------------------------------------------\n");
            }
            else printf("Only possible on sorted data\n");

            printf("\n\n");

            //cleanup word array
            free(WordArray.WordArray);
            WordArray.WordArray = NULL;

            //Hash Table:

            //max size of hash table
            int MaxSize = 10000;

            //create the hash table from the list of words
            struct ReturnHash WordHashTable = initHash(FileType, MaxSize);
            if (WordHashTable.WordCount == -1) return -1;

            printf("**Method: Hash Table**\n");
            printf("--------------------------------------------------------------------------\n");
            printf("|Algorithm:                    |StringFound:   |Suggestions:             |\n");
            printf("|------------------------------+---------------+-------------------------|\n");

            //no algorithm
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 0, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Levenshtein Distance
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 1, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Hamming Distance
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 2, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Sørensen–Dice Coefficient
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 3, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Optimal String Alignment Distance
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 4, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Damerau-Levenshtein Distance
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 5, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Jaro Distance
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 6, 1, "");
            printf("|------------------------------+---------------+-------------------------|\n");

            //Jaro-Winkler Distance
            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 7, 1, "");

            printf("--------------------------------------------------------------------------\n");
            printf("\n\n");

            //cleanup hash table
            struct WordNode *tmp, *currentNode;
            for (int i = 0; i < MaxSize; i++)
            {
                if (WordHashTable.HashTable[i].head != NULL && WordHashTable.HashTable[i].tail != NULL)
                {
                    currentNode = WordHashTable.HashTable[i].head;
                    while(currentNode->next != NULL)
                    {
                        tmp = currentNode->next;
                        free(currentNode);
                        currentNode = tmp;
                    }
                    tmp = currentNode;
                    free(tmp);
                }
            }
            

            //user input (just to confirm that the user can return back to the previous menu)
            char ActionChoice2[128];
            printf("Type anything to enter another word: ");
            scanf("%s", ActionChoice2);
        }
    }

    return 0;
}

int AlgorithmAssessor(int FileType)
{
    //word array for storing all the words
    char **CorrectspelledWordArray;
    CorrectspelledWordArray = malloc(6000 * sizeof(char*));
    struct MisspelledWord* MisspelledWordArray = (struct MisspelledWord*) malloc(6000 * sizeof(struct MisspelledWord));
    
    int CorrectWordCount = 0;
    int IncorrectWordCount = 0;

    FILE* fp;

    for (int i = 0; i < 2; i++)
    {
        
        //open the three mispelled word files
        if (i == 0) fp = fopen("../corpus/Words-Misspelled/aspell.dat.txt","r");
        if (i == 1) fp = fopen("../corpus/Words-Misspelled/wikipedia.dat.txt","r");

        char nextChar = ' ';
        char nextString[50];

        //for testing
        int lineCount = 0;
        
        int charCount = 0;

        //for checking if a word only has chars from a - z
        int ValidCheck = 0;
        //for checking if a correct word is valid or not (all its subsequent incorrect spellings will be ignored)
        int CorrectWordCheck = 0;

        //read all the words into a large list 
        while (nextChar != EOF)
        {
            //get next char
            nextChar = fgetc(fp);

            if (nextChar != '\n')
            {
                //if any invalid chars are in the word, update the count
                if ((int)nextChar < 97 || (int)nextChar > 122) 
                {
                    if (nextChar != '$') ValidCheck++;
                }
                nextString[charCount] = nextChar;
                charCount++;
            }

            else if (nextChar == '\n')
            {
                //complete word
                nextString[charCount] = '\0';
                //printf("%s\n", nextString);

                //if this is a correctly spelled word which is valid
                if (ValidCheck == 0 && nextString[0] == '$')
                {
                    //check if its a duplicate or not
                    int duplicateCheck = 0;
                    for (int i = 0; i < CorrectWordCount; i++)
                    {
                        if (!strcmp(CorrectspelledWordArray[i], nextString))
                        {
                            duplicateCheck++;
                            break;
                        }
                    }
                    //if not, then add to list and update count
                    if (duplicateCheck == 0)
                    {
                        CorrectspelledWordArray[CorrectWordCount] = malloc(50 * sizeof(char));
                        strcpy(CorrectspelledWordArray[CorrectWordCount], nextString);
                        CorrectWordCount++;
                        CorrectWordCheck = 0;
                    }
                    //otherwise, disregard all its mispellings
                    else
                    {
                        CorrectWordCheck = 1;
                    }
                }
                //if a correctly spelled word is invalid, disregard its mispelled children words
                else if (ValidCheck == 1 && nextString[0] == '$') CorrectWordCheck++;
                //for valid mispelled words
                else if (ValidCheck == 0 && nextString[0] != '$' && CorrectWordCheck == 0)
                {
                    //check if its a duplicate
                    int duplicateCheck = 0;
                    for (int i = 0; i < IncorrectWordCount; i++)
                    {
                        if (!strcmp(MisspelledWordArray[i].Word, nextString))
                        {
                            duplicateCheck++;
                            break;
                        }
                    }
                    //if not, add it to the list
                    if (duplicateCheck == 0)
                    {
                        strcpy(MisspelledWordArray[IncorrectWordCount].Word, nextString);
                        MisspelledWordArray[IncorrectWordCount].index = CorrectWordCount - 1;
                        IncorrectWordCount++;
                    }

                }
                
                //update values after word is processed
                lineCount++;
                charCount = 0;
                ValidCheck = 0;
            }
        }

        //close the file each time
        fclose(fp);
    
    }
    //begin testing loop
    //printf("Correct Words: %d, Incorrect Words: %d\n", CorrectWordCount, IncorrectWordCount);
    
    //make a number of variables for calculating the average time and suggestion number for each algorithm
    //levenshtein distance
    int LevenshteinDistanceMissCount[3] = {0, 0, 0};

    //Hamming distance
    int HammingDistanceMissCount[3] = {0, 0, 0};

    //sorensen-dice coefficient
    int SorensenDiceMissCount[3] = {0, 0, 0};

    //optimal string alignment distance
    int OptimalStringAlignmentMissCount[3] = {0, 0, 0};

    //damerau-levenshtein distance
    int DamerauLevensheinMissCount[3] = {0, 0, 0};

    //Jaro similary
    int JaroDistanceMissCount[3] = {0, 0, 0};

    //JaroWinklerSimilarity
    int JaroWinklerDistanceMissCount[3] = {0, 0, 0};
    

    //first generate the wordlist data structure for linear and binary searching
    struct WordsListing WordArray = WordFetcher(FileType);
    if (WordArray.WordCount == -1) return -1;
    
    //max size of hash table
    int MaxSize = 10000;

    //create the hash table from the list of words
    struct ReturnHash WordHashTable = initHash(FileType, MaxSize);
    if (WordHashTable.WordCount == -1) return -1;

    FILE *WFP;

    if (FileType == 1) WFP = fopen("../AutomatedTestingResults/CProgramResultsType0.txt", "w");
    else if (FileType == 3) WFP = fopen("../AutomatedTestingResults/CProgramResultsType1.txt", "w");
    if (WFP == NULL)
    {
        printf("Error: unable to open word file\n");
        return -1;
    }

    fprintf(WFP, "CorrectCount: %d, IncorrectCount: %d\n", CorrectWordCount, IncorrectWordCount);

    fprintf(WFP, "suggestion numbers and runtimes\n");
    for (int i = 0; i < IncorrectWordCount; i++)
    {
        printf("%s -> %s\n", MisspelledWordArray[i].Word, CorrectspelledWordArray[MisspelledWordArray[i].index]);
        char MasterWord[50]; 
        strcpy(MasterWord, CorrectspelledWordArray[MisspelledWordArray[i].index] + 1);
        //printf("%s\n", MasterWord);
        
        //tmp structure to hold data returned from functions
        struct TimeAndSuggestion returnedValues;

        //Linear

        //no algorithm
        returnedValues = linearCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 0, 2, MasterWord);
        
        fprintf(WFP, "L/NO: %lf\n", returnedValues.TimeTaken);

        //Levenshtein Distance
        returnedValues = linearCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 1, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) LevenshteinDistanceMissCount[0]++;
        
        fprintf(WFP, "L/LD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Hamming Distance
        returnedValues = linearCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 2, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) HammingDistanceMissCount[0]++;

        fprintf(WFP, "L/HD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);
    
        //Sørensen–Dice Coefficient
        returnedValues = linearCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 3, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) SorensenDiceMissCount[0]++;

        fprintf(WFP, "L/SD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Optimal String Alignment Distance
        returnedValues = linearCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 4, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) OptimalStringAlignmentMissCount[0]++;

        fprintf(WFP, "L/OD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Damerau-Levenshtein Distance
        returnedValues = linearCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 5, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) DamerauLevensheinMissCount[0]++;

        fprintf(WFP, "L/DL: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);


        //Jaro Distance
        returnedValues = linearCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 6, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) JaroDistanceMissCount[0]++;

        fprintf(WFP, "L/JD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);


        //Jaro-Winkler Distance
        returnedValues = linearCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 7, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) JaroWinklerDistanceMissCount[0]++;

        fprintf(WFP, "L/LW: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);


        //Binary:
        //need a sorted array to use this
        if (FileType == 3)
        {
            //no algorithm
            returnedValues = binaryCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 0, 2, MasterWord);
            
            fprintf(WFP, "B/NO: %lf\n", returnedValues.TimeTaken);

            //Levenshtein Distance
            returnedValues = binaryCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 1, 2, MasterWord);
            if (returnedValues.SuggestionNumber == 0) LevenshteinDistanceMissCount[1]++;
            
            fprintf(WFP, "B/LD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

            //Hamming Distance
            returnedValues = binaryCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 2, 2, MasterWord);
            if (returnedValues.SuggestionNumber == 0) HammingDistanceMissCount[1]++;

            fprintf(WFP, "B/HD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

            //Sørensen–Dice Coefficient
            returnedValues = binaryCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 3, 2, MasterWord);
            if (returnedValues.SuggestionNumber == 0) SorensenDiceMissCount[1]++;
            
            fprintf(WFP, "B/SD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

            //Optimal String Alignment Distance
            returnedValues = binaryCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 4, 2, MasterWord);
            if (returnedValues.SuggestionNumber == 0) OptimalStringAlignmentMissCount[1]++;

            fprintf(WFP, "B/OS: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

            //Damerau-Levenshtein Distance
            returnedValues = binaryCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 5, 2, MasterWord);
            if (returnedValues.SuggestionNumber == 0) DamerauLevensheinMissCount[1]++;

            fprintf(WFP, "B/DL: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

            //Jaro Distance
            returnedValues = binaryCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 6, 2, MasterWord);
            if (returnedValues.SuggestionNumber == 0) JaroDistanceMissCount[1]++;
            
            fprintf(WFP, "B/JD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

            //Jaro-Winkler Distance
            returnedValues = binaryCheck(WordArray.WordArray, WordArray.WordCount, MisspelledWordArray[i].Word, 7, 2, MasterWord);
            if (returnedValues.SuggestionNumber == 0) JaroWinklerDistanceMissCount[1]++;

            fprintf(WFP, "B/JW: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);
        }

        //Hashing

        //no algorithm
        returnedValues = HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, MisspelledWordArray[i].Word, 0, 2, MasterWord);

        fprintf(WFP, "H/NO: %lf\n", returnedValues.TimeTaken);

        //Levenshtein Distance
        returnedValues = HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, MisspelledWordArray[i].Word, 1, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) LevenshteinDistanceMissCount[2]++;

        fprintf(WFP, "H/LD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Hamming Distance
        returnedValues = HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, MisspelledWordArray[i].Word, 2, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) HammingDistanceMissCount[2]++;

        fprintf(WFP, "H/HD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Sørensen–Dice Coefficient
        returnedValues = HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, MisspelledWordArray[i].Word, 3, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) SorensenDiceMissCount[2]++;

        fprintf(WFP, "H/SD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Optimal String Alignment Distance
        returnedValues = HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, MisspelledWordArray[i].Word, 4, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) OptimalStringAlignmentMissCount[2]++;

        fprintf(WFP, "H/OS: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Damerau-Levenshtein Distance
        returnedValues = HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, MisspelledWordArray[i].Word, 5, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) DamerauLevensheinMissCount[2]++;

        fprintf(WFP, "H/DL: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Jaro Distance
        returnedValues = HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, MisspelledWordArray[i].Word, 6, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) JaroDistanceMissCount[2]++;

        fprintf(WFP, "H/JD: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);

        //Jaro-Winkler Distance
        returnedValues = HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, MisspelledWordArray[i].Word, 7, 2, MasterWord);
        if (returnedValues.SuggestionNumber == 0) JaroWinklerDistanceMissCount[2]++;

        fprintf(WFP, "H/JW: %ld : %lf\n", returnedValues.SuggestionNumber, returnedValues.TimeTaken);
    }
    //cleanup word array
    free(WordArray.WordArray);
    WordArray.WordArray = NULL;

    //cleanup hash table
    struct WordNode *tmp, *currentNode;
    for (int i = 0; i < MaxSize; i++)
    {
        if (WordHashTable.HashTable[i].head != NULL && WordHashTable.HashTable[i].tail != NULL)
        {
            currentNode = WordHashTable.HashTable[i].head;
            while(currentNode->next != NULL)
            {
                tmp = currentNode->next;
                free(currentNode);
                currentNode = tmp;
            }
            tmp = currentNode;
            free(tmp);
        }
    }
    fprintf(WFP, "Total Miss Counts:\n");
    fprintf(WFP, "LD: L:%d, B:%d, H:%d\n", LevenshteinDistanceMissCount[0], LevenshteinDistanceMissCount[1],LevenshteinDistanceMissCount[2]);
    fprintf(WFP, "HD: L:%d, B:%d, H:%d\n", HammingDistanceMissCount[0], HammingDistanceMissCount[1],HammingDistanceMissCount[2]);
    fprintf(WFP, "SD: L:%d, B:%d, H:%d\n", SorensenDiceMissCount[0], SorensenDiceMissCount[1],SorensenDiceMissCount[2]);
    fprintf(WFP, "OS: L:%d, B:%d, H:%d\n", OptimalStringAlignmentMissCount[0], OptimalStringAlignmentMissCount[1],OptimalStringAlignmentMissCount[2]);
    fprintf(WFP, "DL: L:%d, B:%d, H:%d\n", DamerauLevensheinMissCount[0], DamerauLevensheinMissCount[1],DamerauLevensheinMissCount[2]);
    fprintf(WFP, "JD: L:%d, B:%d, H:%d\n", JaroDistanceMissCount[0], JaroDistanceMissCount[1],JaroDistanceMissCount[2]);
    fprintf(WFP, "JW: L:%d, B:%d, H:%d\n", JaroWinklerDistanceMissCount[0], JaroWinklerDistanceMissCount[1],JaroWinklerDistanceMissCount[2]);

    //cleanup
    free(CorrectspelledWordArray);
    CorrectspelledWordArray = NULL;

    fclose(WFP);

    char FinalAction[128];
    printf("Type anything to enter another word: ");
    scanf("%s", FinalAction);
    return 0 ;
}