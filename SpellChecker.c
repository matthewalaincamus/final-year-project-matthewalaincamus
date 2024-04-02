#include "SpellChecker.h"

//skeleton functions for use later
struct WordsListing WordFetcher(int FileType);
struct WordHashItem *initHash(int FileType);
int linearCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, clock_t Start);
int binaryCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, clock_t Start);
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[25]);
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

        if (FileType == 0)
        {
            printf("text with duplicate\n\n");
        }
        else if (FileType == 1)
        {
            printf("text without duplicates\n\n");
        }
        else if (FileType== 2)
        {
            printf("text with duplicates sorted in alphabetical order\n\n");
        }
        else if (FileType == 3)
        {
            printf("text without duplicates sorted in alphabetical order\n\n");
        }

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

            if(!strcmp(ActionChoice, "?"))
            {
                return 0;
            }

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

        struct WordsListing WordArray = WordFetcher(FileType);
        if (WordArray.WordCount == -1) return -1;

        //start timer
        clock_t Start = clock();

        //linear searching = 0
        if (CheckType == 0) linearCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, AlgorithmCheck, Start);

        //binary searching = 1
        else if (CheckType == 1) binaryCheck(WordArray.WordArray, WordArray.WordCount, ActionChoice, AlgorithmCheck, Start); 

        printf("\n");

        //user input (just to confirm that the user can return back to the previous menu)
        char ActionChoice2[128];
        printf("Type anything to enter another word: ");
        scanf("%s", ActionChoice2);

        //cleanup
        free(WordArray.WordArray);
        WordArray.WordArray = NULL;
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
        fp = fopen("./corpus/LWD.txt", "r");
    }
    else if(FileType == 1)
    {
        fp = fopen("./corpus/LWND.txt", "r");
    }
    else if(FileType == 2)
    {
        fp = fopen("./corpus/LWDS.txt", "r");
    }
    else if(FileType == 3)
    {
        fp = fopen("./corpus/LWNDS.txt", "r");
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

struct WordHashItem *initHash(int FileType)
{
    FILE* fp;

    int MaxSize = 10000;

    if (FileType == 0)
    {
        fp = fopen("./corpus/LWD.txt", "r");
    }
    else if(FileType == 1)
    {
        fp = fopen("./corpus/LWND.txt", "r");
    }
    else if(FileType == 2)
    {
        fp = fopen("./corpus/LWDS.txt", "r");
    }
    else if(FileType == 3)
    {
        fp = fopen("./corpus/LWNDS.txt", "r");
    }
    else
    {
        printf("Error: bad fileflag found\n");
        return;
    }

    if (fp == NULL)
    {
        printf("Error: unable to open word file\n");
        return;
    }

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
            printf("%s\n", nextString);
            
            WordCount++;
            
            lineCount++;
            charCount = 0;
        }
    }

    fclose(fp);

    return;
}

//linear search checker
int linearCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, clock_t Start)
{
    int WordFlag = 0;
    int lineCount = 0;

    //for suggestion calculations
    int AlgorithmArray [WordCount];

    //if the word is a single letter, no need to do any further checks as they are always valid
    if (strlen(wordString) == 1)
    {
        printf("Your word exists in the word list\n");
        WordFlag++;
    }
    else
    {
        for (int i = 0; i < WordCount; i++)
        {
            if(!strcmp(WordArray[i], wordString))
            {
                printf("Your word exists in the word list\n");
                WordFlag++;

                clock_t TimeDif = clock() - Start;
                double TimeTaken = (double)TimeDif / CLOCKS_PER_SEC;

                printf("Operation took: %f Seconds to complete\n\n", TimeTaken);
                break;
            }
            lineCount++;
            
            //if lev is active
            if (AlgorithmCheck == 1)
            {
                AlgorithmArray[i] = LevenshteinDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            }
            else if (AlgorithmCheck == 2)
            {
                AlgorithmArray[i] = HammingDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            }
            else if (AlgorithmCheck == 3)
            {
                AlgorithmArray[i] = SorensenDiceCoefficient(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            }
            else if (AlgorithmCheck == 4)
            {
                AlgorithmArray[i] = OptimalStringAlignmentDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            }
            else if (AlgorithmCheck == 5)
            {
                AlgorithmArray[i] = DamerauLevenshteinDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            }
            else if (AlgorithmCheck == 6)
            {
                AlgorithmArray[i] = JaroSimilarity(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            }
            else if (AlgorithmCheck == 7)
            {
                AlgorithmArray[i] = JaroWinklerSimilarity(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
            }
        }
    }

    //if word wasn't found, get suggestions
    if (WordFlag == 0)
    {
        printf("Your word doesn't exist in the word list\n");

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

                printf("Suggestion %i: %s\n", i+1, WordArray[CurrentMinIndex]);
            }
        }

        clock_t TimeDif = clock() - Start;
        double TimeTaken = (double)TimeDif / CLOCKS_PER_SEC;

        printf("Operation took: %f Seconds to complete\n\n", TimeTaken);
    }
    return 0;
}

//binary search checkcer
int binaryCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, clock_t Start)
{
    //perform the binary search
    int binaryCheck = binarySearch(WordArray, 0, WordCount - 1, wordString);
    if (binaryCheck == 0) 
    {
        printf("Your word doesn't exist in the word list\n");

        if (AlgorithmCheck != 0)
        {
            //for suggestion calculations
            int AlgorithmArray [WordCount];

            //get all the levenshtein distances from string linearly
            for (int i = 0; i < WordCount; i++)
            {   
                //if lev is active
                if (AlgorithmCheck == 1)
                {
                    AlgorithmArray[i] = LevenshteinDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                }
                else if(AlgorithmCheck == 2)
                {
                    AlgorithmArray[i] = HammingDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                }
                else if (AlgorithmCheck == 3)
                {
                    AlgorithmArray[i] = SorensenDiceCoefficient(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                }
                else if (AlgorithmCheck == 4)
                {
                    AlgorithmArray[i] = OptimalStringAlignmentDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                }
                else if (AlgorithmCheck == 5)
                {
                    AlgorithmArray[i] = DamerauLevenshteinDistance(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                }
                else if (AlgorithmCheck == 6)
                {
                    AlgorithmArray[i] = JaroSimilarity(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                }
                else if (AlgorithmCheck == 7)
                {
                    AlgorithmArray[i] = JaroWinklerSimilarity(wordString, WordArray[i], strlen(wordString), strlen(WordArray[i]));
                }
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

                    printf("Suggestion %i: %s\n", i+1, WordArray[CurrentMinIndex]);
                }
            }
        }
    }
    else if (binaryCheck == 1) printf("Your word exists in the word list\n");
    else
    {
        printf("Error: problem with binary search\n");
    }

    //get the time taken
    clock_t TimeDif = clock() - Start;
    double TimeTaken = (double)TimeDif / CLOCKS_PER_SEC;

    printf("Operation took: %f Seconds to complete\n\n", TimeTaken);
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
    if (len1 != len2) return 50; 

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
    //number of bigraphs that match
    double matches = 0;

    int i = 0, j = 0;

    //loop through, getting the number of bigraphs that match and updating the count
    while (i < len1 - 1 && j < len2 - 1)
    {
        char bigraph1[3] = {string1[i], string2[i+1], '\0'};
        char bigraph2[3] = {string2[j], string2[j+1], '\0'};

        if (!strcmp(bigraph1, bigraph2)) matches++;

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

    //travers the first string
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

    return 0;
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