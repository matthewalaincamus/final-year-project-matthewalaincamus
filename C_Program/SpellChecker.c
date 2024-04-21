#include "SpellChecker.h"

//skeleton functions for use later
struct WordsListing WordFetcher(int FileType);
struct ReturnHash initHash(int FileType, int MaxSize);
int linearCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, struct timeval Start, int TestFlag);
int binaryCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, struct timeval Start, int TestFlag);
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[25]);
int HashChecker(struct WordHashItem *HashTable, int MaxSize, int WordCount, char wordString[], int AlgorithmCheck, struct timeval Start, int TestFlag);
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
        printf("- maximum length string of 25 characters\n");
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
                if (i > 24) WordCheck++;

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
            struct timeval Start;
            gettimeofday(&Start, 0);

            //linear searching = 0
            if (CheckType == 0) linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, AlgorithmCheck, Start, 0);

            //binary searching = 1
            else if (CheckType == 1) binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, AlgorithmCheck, Start, 0); 
        
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

            //start timer
            struct timeval Start;
            gettimeofday(&Start, 0);

            HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, AlgorithmCheck, Start, 0);

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
    char nextString[25];

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

            WordArray[WordCount] = malloc(25 * sizeof(char));
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
    char nextString[25];

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
int linearCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, struct timeval Start, int TestFlag)
{

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
            }
        }
        else if (AlgorithmCheck == 0 && TestFlag == 1) printf("|%-25s|\n", "N/A");
    }
    else if (WordFlag > 0 && TestFlag == 1) printf("|%-15s|%-25s|\n", "True", "");

    //get the time of completion
    struct timeval Stop;
    gettimeofday(&Stop, 0);

    float TimeTaken = (Stop.tv_sec - Start.tv_sec) * 1000.f + (Stop.tv_usec - Start.tv_usec) / 1000.f;

    if (TestFlag == 0) printf("Operation took: %f miliseconds to complete\n\n", TimeTaken);
    else if (TestFlag == 1) printf("|Time to complete: %-10f ms%-41s|\n",  TimeTaken, "");
    return 0;
}

//binary search checker
int binaryCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, struct timeval Start, int TestFlag)
{
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
    else
    {
        printf("Error: problem with binary search\n");
    }

    //get the time taken
    struct timeval Stop;
    gettimeofday(&Stop, 0);

    float TimeTaken = (Stop.tv_sec - Start.tv_sec) * 1000.f + (Stop.tv_usec - Start.tv_usec) / 1000.f;

    if (TestFlag == 0) printf("Operation took: %f miliseconds to complete\n\n", TimeTaken);
    else if (TestFlag == 1) printf("|Time to complete: %-10f ms%-41s|\n",  TimeTaken, "");
    return 0;
}

//hash table checker
int HashChecker(struct WordHashItem *HashTable, int MaxSize, int WordCount, char wordString[], int AlgorithmCheck, struct timeval Start, int TestFlag)
{
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
            }
        }
        else if (AlgorithmCheck == 0 && TestFlag == 1) printf("|%-25s|\n", "N/A");
    }
    else if (WordFlag > 0 && TestFlag == 1) printf("|%-15s|%-25s|\n", "True", "");

    //get the time of completion
    struct timeval Stop;
    gettimeofday(&Stop, 0);

    float TimeTaken = (Stop.tv_sec - Start.tv_sec) * 1000.f + (Stop.tv_usec - Start.tv_usec) / 1000.f;

    if (TestFlag == 0) printf("Operation took: %f miliseconds to complete\n\n", TimeTaken);
    else if (TestFlag == 1) printf("|Time to complete: %-10f ms%-41s|\n",  TimeTaken, "");

    return 0;
}

//binary search recursive algorithm
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[25])
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
    //empty string1
    if (len1 == 0) return len2;

    //empty string2
    if (len2 == 0) return len1;

    if(string1[len1 - 1] == string2[len2 - 1])
    {
        return LevenshteinDistance(string1, string2, len1 - 1, len2 - 1);
    }
    
    return 1 + Min( 
        //insert
        LevenshteinDistance(string1, string2, len1, len2 -1),
        Min(
            //Remove
            LevenshteinDistance(string1, string2, len1 - 1, len2),
            //Replace
            LevenshteinDistance(string1, string2, len1 - 1, len2 - 1)
        ));
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
    //make an array of the size of the alphabet for calculations
    int alphabet[26];

    for (int i = 0; i < 26; i++) alphabet[i] = 0;

    //array to store results of calculations
    int CalculationArray[len1+2][len2+2];

    //get the maximum distance
    int maxDist = len1 + len2;
    CalculationArray[0][0] = maxDist;

    //setup the starting values to the array
    for (int i = 1; i <= len1; i++)
    {
        CalculationArray[i][0] = maxDist;
        CalculationArray[i][1] = i-1;
    }
    for (int j = 1; j <= len2; j++)
    {
        CalculationArray[0][j] = maxDist;
        CalculationArray[1][j] = j-1;
    }

    //main calculation loop
    for (int i = 0; i <= len1; i++)
    {
        int db = 0;
        for (int j = 0; j <= len2; j++)
        {
            int k = alphabet[(int)string2[j] % 97];
            int l = db;
            int cost;

            if (string1[i] == string2[j])
            {
                cost = 0;
                db = j;
            }
            else cost = 1;

            CalculationArray[i+2][j+2] = Min( Min(CalculationArray[i+1][j+1]+cost, //substitution
                                                    CalculationArray[i+2][j+1]+1 ), //insertion
                                              Min(CalculationArray[i+1][j+2]+1, //deletion
                                                    CalculationArray[k][l]+(i-k+1)+1+(j-l+1)) ); //transpostion
        alphabet[(int)string1[i]%97] = i+2;
        }
    }

    return CalculationArray[len1+2][len2+2];
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
        printf("- maximum length string of 25 characters\n");
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
                if (i > 24) WordCheck++;

                int charToInt = (int)ActionChoice[i];

                if (ActionChoice[i] == '\0') break;
                else if (isdigit(ActionChoice[i])) WordCheck++;
                else if (charToInt < 97 || charToInt > 122) WordCheck++;
            }

            if (WordCheck > 0) printf("Please enter a valid word\n");
            else inputCheck = 0;
        }

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
        struct timeval Start;
        gettimeofday(&Start, 0);
        linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 0, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Levenshtein Distance
        gettimeofday(&Start, 0);
        linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 1, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Hamming Distance
        gettimeofday(&Start, 0);
        linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 2, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Sørensen–Dice Coefficient
        gettimeofday(&Start, 0);
        linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 3, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Optimal String Alignment Distance
        gettimeofday(&Start, 0);
        linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 4, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Damerau-Levenshtein Distance
        gettimeofday(&Start, 0);
        linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 5, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Jaro Distance
        gettimeofday(&Start, 0);
        linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 6, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Jaro-Winkler Distance
        gettimeofday(&Start, 0);
        linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 7, Start, 1);

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
            gettimeofday(&Start, 0);
            binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 0, Start, 1);
            printf("|------------------------------+---------------+-------------------------|\n");

            //Levenshtein Distance
            gettimeofday(&Start, 0);
            binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 1, Start, 1);
            printf("|------------------------------+---------------+-------------------------|\n");

            //Hamming Distance
            gettimeofday(&Start, 0);
            binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 2, Start, 1);
            printf("|------------------------------+---------------+-------------------------|\n");

            //Sørensen–Dice Coefficient
            gettimeofday(&Start, 0);
            binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 3, Start, 1);
            printf("|------------------------------+---------------+-------------------------|\n");

            //Optimal String Alignment Distance
            gettimeofday(&Start, 0);
            binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 4, Start, 1);
            printf("|------------------------------+---------------+-------------------------|\n");

            //Damerau-Levenshtein Distance
            gettimeofday(&Start, 0);
            binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 5, Start, 1);
            printf("|------------------------------+---------------+-------------------------|\n");

            //Jaro Distance
            gettimeofday(&Start, 0);
            binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 6, Start, 1);
            printf("|------------------------------+---------------+-------------------------|\n");

            //Jaro-Winkler Distance
            gettimeofday(&Start, 0);
            binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, 7, Start, 1);

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
        gettimeofday(&Start, 0);
        HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 0, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Levenshtein Distance
        gettimeofday(&Start, 0);
        HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 1, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Hamming Distance
        gettimeofday(&Start, 0);
        HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 2, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Sørensen–Dice Coefficient
        gettimeofday(&Start, 0);
        HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 3, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Optimal String Alignment Distance
        gettimeofday(&Start, 0);
        HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 4, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Damerau-Levenshtein Distance
        gettimeofday(&Start, 0);
        HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 5, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Jaro Distance
        gettimeofday(&Start, 0);
        HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 6, Start, 1);
        printf("|------------------------------+---------------+-------------------------|\n");

        //Jaro-Winkler Distance
        gettimeofday(&Start, 0);
        HashChecker(WordHashTable.HashTable, MaxSize, WordHashTable.WordCount, ActionChoice, 7, Start, 1);

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

    return 0;
}