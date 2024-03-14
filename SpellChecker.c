#include "SpellChecker.h"

struct WordsListing WordFetcher(int FileType);
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[25]);
int LevenshteinChecker(char string1[], char string2[], int len1, int len2);
int Min(int a, int b);

int simpleCheckLinear(int FileType, int LevCheck)
{
    //check to see if the user wants to return to the main menu
    int returnCheck = 1;

    //will loop till the user exits via typing z or Z on the keyboard in the main menu
    while (returnCheck == 1)
    {
        system("cls");

        if (LevCheck == 0) printf("Simple Check Spell Check:\n");
        else if (LevCheck == 1) printf("Simple Check Spell Check with levenshtein distance based suggestions:\n");
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

        //testing
        if (LevCheck == 1)
        {
            char testString[] = "apples";
            printf("String1: %s, String2: %s, value: %d\n", 
            ActionChoice, 
            testString, 
            LevenshteinChecker(ActionChoice, testString, strlen(ActionChoice), strlen(testString)));
        }

        //start timer
        clock_t Start = clock();

        struct WordsListing WordArray = WordFetcher(FileType);
        if (WordArray.WordCount == -1) return -1;

        int WordFlag = 0;
        int lineCount = 0;

        //for lev calculations
        int LevArray [WordArray.WordCount];

        //if the word is a single letter, no need to do any further checks as they are always valid
        if (strlen(ActionChoice) == 1)
        {
            printf("Your word exists in the word list\n");
            WordFlag++;
        }
        else
        {
            for (int i = 0; i < WordArray.WordCount; i++)
            {
                if(!strcmp(WordArray.WordArray[i], ActionChoice))
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
                if (LevCheck == 1)
                {
                    LevArray[i] = LevenshteinChecker(ActionChoice, WordArray.WordArray[i], strlen(ActionChoice), strlen(WordArray.WordArray[i]));
                }

            }
        }

        if (WordFlag == 0)
        {
            printf("Your word doesn't exist in the word list\n");

            //beginning of levenstien distance calculations
            for (int i = 0; i < 10; i++)
            {
                int CurrentMin = 50;
                int CurrentMinIndex = 0;

                for(int j = 0; j < WordArray.WordCount; j++)
                {
                    if (LevArray[j] < CurrentMin)
                    {
                        CurrentMin = LevArray[j];
                        CurrentMinIndex = j;
                    }
                }

                LevArray[CurrentMinIndex] = 50;

                printf("Suggestion %i: %s\n", i+1, WordArray.WordArray[CurrentMinIndex]);
            }

            clock_t TimeDif = clock() - Start;
            double TimeTaken = (double)TimeDif / CLOCKS_PER_SEC;

            printf("Operation took: %f Seconds to complete\n\n", TimeTaken);
        }

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

int simpleCheckBinary(int FileType)
{
    //check to see if the user wants to return to the main menu
    int returnCheck = 1;

    //will loop till the user exits via typing z or Z on the keyboard in the main menu
    while (returnCheck == 1)
    {
        system("cls");

        printf("Simple Check Spell Check:\n");
        printf("**Current Filetype selected: ");

        if (FileType== 2)
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

        //start timer
        clock_t Start = clock();

        struct WordsListing WordArray = WordFetcher(FileType);
        if (WordArray.WordCount == -1) return -1;

        int binaryCheck = binarySearch(WordArray.WordArray, 0, WordArray.WordCount - 1, ActionChoice);
        if (binaryCheck == 0) printf("Your word doesn't exist in the word list\n");
        else if (binaryCheck == 1) printf("Your word exists in the word list\n");
        else
        {
            printf("Error: problem with binary search\n");
        }

        clock_t TimeDif = clock() - Start;
        double TimeTaken = (double)TimeDif / CLOCKS_PER_SEC;

        printf("Operation took: %f Seconds to complete\n\n", TimeTaken);

        //cleanup
        free(WordArray.WordArray);
        WordArray.WordArray = NULL;

        //user input (just to confirm that the user can return back to the previous menu)
        char ActionChoice2[128];
        printf("Type anything to enter another word: ");
        scanf("%s", ActionChoice2);
    }

    return 0;
}

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

int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[25])
{
    if (rightValue >= leftValue)
    {
        int middleValue = leftValue + (rightValue - leftValue) / 2;
        //printf("LeftValue: %i, MiddleValue: %i, RightValue: %i\n", leftValue, middleValue, rightValue);
        //printf("WordArray: %s, Word to find: %s\n", WordArray[middleValue], wordString);

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

int LevenshteinChecker(char string1[], char string2[], int len1, int len2)
{
    //empty string1
    if (len1 == 0) return len2;

    //empty string2
    if (len2 == 0) return len1;

    if(string1[len1 - 1] == string2[len2 - 1])
    {
        return LevenshteinChecker(string1, string2, len1 - 1, len2 - 1);
    }
    
    return 1 + Min( 
        //insert
        LevenshteinChecker(string1, string2, len1, len2 -1),
        Min(
            //Remove
            LevenshteinChecker(string1, string2, len1 - 1, len2),
            //Replace
            LevenshteinChecker(string1, string2, len1 - 1, len2 - 1)
        ));
}

int Min(int a, int b)
{
    if (a < b) return a;
    else return b;
}