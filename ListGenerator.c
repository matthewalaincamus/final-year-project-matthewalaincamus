#include "ListGenerator.h"

//**function for making list of words from corpus
//if duplicateFlag == 0 (no duplicates), if duplicateFlag == 1 (duplicates)
//if sortFlag = 0 (no sort), if sortFlag = 1 (sort alphabetically), if sortFlag == 2 (sort by frequency)
int DataReader(int duplicateFlag, int sortFlag)
{
    //clear the screen so that only relevant system text is shown
    system("cls");

    //show user options for the corpus
    printf("Please type a letter from A to K as two which text you would like (has to be capitals):\n");
    printf("Type Z, if you wish to return to the previous screen\n");

    int letterCheck = 0;
    char ActionChoice1[128];
    while(letterCheck == 0)
    {
        //user input (just to confirm that the user can return back to the previous menu)
        printf(": ");
        scanf("%s", ActionChoice1);

        if(strlen(ActionChoice1) == 1)
        {
            int charToInt = (int)ActionChoice1[0];
            if (charToInt >= 65 && charToInt <= 75)
            {
                break;
            }
            else if(!strcmp(ActionChoice1, "Z") || !strcmp(ActionChoice1, "z"))
            {
                return 0;
            }
            else
            {
                printf("please input a letter inside the required range and is capitalised\n");
            }
        }
        else
        {
            printf("your choice needs to be one letter\n");
        }
    }

    //start timer
    clock_t Start = clock();

    printf("***Generating list***\n");
    printf("LOADING...\n");

    //make a string array to store the words for duplicate checking
    char **WordArray;
    WordArray = malloc(20000000 * sizeof(char*));
    int WordCount = 0;

    FILE* Wfp;

    //this is the file where all the words are deposited
    if (sortFlag == 0 && duplicateFlag == 1)Wfp = fopen("./corpus/LWD.txt", "w");
    else if (sortFlag == 1 && duplicateFlag == 1)Wfp = fopen("./corpus/LWDS.txt", "w");
    else if (sortFlag == 0 && duplicateFlag == 0)Wfp = fopen("./corpus/LWND.txt", "w");
    else if (sortFlag == 1 && duplicateFlag == 0)Wfp = fopen("./corpus/LWNDS.txt", "w");
    else
    {
        printf("Error: invalid sortCheck or duplicateCheck value given\n");
        return -1;
    }

    if (Wfp == NULL)
    {
        printf("Error: unable to open word file\n");
        return -1;
    }

    struct dirent *dp;
    char directoryName[50] = "./corpus/Texts/";
    strcat(directoryName, ActionChoice1);
    //printf("%s\n", directoryName);

    DIR *dir = opendir(directoryName);
    if (!dir)
    {
        printf("Error: directory doesn't exist\n");
        return -1;
    }
    else
    {
        while ((dp = readdir(dir)) != NULL)
        {
            if (!(!strcmp(dp->d_name,"..") || !strcmp(dp->d_name, ".")))
            {
                char innnerDirectoryName[50];
                strcpy(innnerDirectoryName, directoryName);
                strcat(innnerDirectoryName, "/");
                strcat(innnerDirectoryName, dp->d_name);
                //printf("%s\n", innnerDirectoryName);

                struct dirent *innerdp;
                DIR *innerdir = opendir(innnerDirectoryName);
                if(!innerdir)
                {
                    printf("Error: directory doesn't exist\n");
                    return -1;
                }
                else
                {
                    while((innerdp = readdir(innerdir)) != NULL)
                    {
                        if (!(!strcmp(innerdp->d_name,"..") || !strcmp(innerdp->d_name, ".")))
                        {
                            char FinalDirectoryName[50];
                            strcpy(FinalDirectoryName, innnerDirectoryName);
                            strcat(FinalDirectoryName, "/");
                            strcat(FinalDirectoryName, innerdp->d_name);

                            //print the files as they load
                            printf("%s\n", FinalDirectoryName);

                            FILE* fp;
                            char nextChar = ' ';

                            fp = fopen(FinalDirectoryName, "r");

                            if (fp == NULL)
                            {
                                printf("Error: unknown xml file\n");
                                return -1;
                            }
                            else
                            {
                                //line count just used for testing purposes
                                int lineCount = 1;

                                while(nextChar != EOF)
                                {
                                    nextChar = fgetc(fp);
                                    if (nextChar == '\n') lineCount++;
                                    //the format of a word in the XML file is:
                                    //<w ... hw="word" ...
                                    //so the following code performs the checks to extract the word from the quotation marks
                                    else if(nextChar == '<')
                                    {
                                        nextChar = fgetc(fp);
                                        if (nextChar == 'w')
                                        {
                                            nextChar = fgetc(fp);
                                            if (nextChar == ' ')
                                            {
                                                //string variable to hold the next word
                                                char nextString[25];

                                                while(nextChar != 'h')
                                                {
                                                    nextChar = fgetc(fp);
                                                }
                                                for (int i=0; i<4; i++)
                                                {
                                                    nextChar = fgetc(fp);
                                                }

                                                //check to see if a digit was found
                                                int breakCheck = 0;

                                                int charCount = 0;
                                                while(nextChar != '"')
                                                {
                                                    if (isdigit(nextChar))
                                                    {
                                                        breakCheck++;
                                                        break;
                                                    }
                                                    else if (nextChar == '/' || nextChar == '-')
                                                    {
                                                        nextString[charCount] = '\0';
                                                        //word shouldn't have numbers or start on anything other than a letter
                                                        //if a word is valid, it is added to the final file of words
                                                        if (breakCheck == 0 && isalpha(nextString[0]))
                                                        {
                                                            int duplicateCheck = 0;

                                                            if (duplicateFlag == 0)
                                                            {
                                                                for (int i = 0; i < WordCount; i++)
                                                                {
                                                                    if (!strcmp(WordArray[i], nextString))
                                                                    {
                                                                        duplicateCheck = 1;
                                                                        break;
                                                                    }
                                                                }
                                                            }

                                                            if((duplicateCheck == 0 && duplicateFlag == 0) || (duplicateFlag == 1))
                                                            {
                                                                WordArray[WordCount] = malloc(25 * sizeof(char));
                                                                strcpy(WordArray[WordCount], nextString);
                                                                WordCount++;

                                                                if (sortFlag == 0)
                                                                {
                                                                    fprintf(Wfp, "%s\n", nextString);
                                                                    //printf("%d: %s\n", lineCount, nextString);
                                                                }
                                                            }
                                                            /*
                                                            else if (duplicateFlag == 1)
                                                            {
                                                                fprintf(Wfp, "%s\n", nextString);
                                                                //printf("%d: %s\n", lineCount, nextString);
                                                                WordCount++;
                                                            }
                                                            */
                                                        }

                                                        charCount = 0;
                                                        breakCheck = 0;
                                                        nextChar = fgetc(fp);
                                                    }
                                                    else
                                                    {
                                                        nextString[charCount] = nextChar;
                                                        nextChar = fgetc(fp);
                                                        charCount++;
                                                    }
                                                }
                                                nextString[charCount] = '\0';
                                                //word shouldn't have numbers or start on anything other than a letter
                                                //if a word is valid, it is added to the final file of words
                                                if (breakCheck == 0 && isalpha(nextString[0]))
                                                {
                                                    int duplicateCheck = 0;

                                                    if (duplicateFlag == 0)
                                                    {
                                                        for (int i = 0; i < WordCount; i++)
                                                        {
                                                            if (!strcmp(WordArray[i], nextString))
                                                            {
                                                                duplicateCheck = 1;
                                                                break;
                                                            }
                                                        }
                                                    }

                                                    if((duplicateCheck == 0 && duplicateFlag == 0) || (duplicateFlag == 1))
                                                    {
                                                        WordArray[WordCount] = malloc(25 * sizeof(char));
                                                        strcpy(WordArray[WordCount], nextString);
                                                        WordCount++;

                                                        if (sortFlag == 0)
                                                        {
                                                            fprintf(Wfp, "%s\n", nextString);
                                                            //printf("%d: %s\n", lineCount, nextString);
                                                        }
                                                    }
                                                    /*
                                                    else if (duplicateFlag == 1)
                                                    {
                                                        fprintf(Wfp, "%s\n", nextString);
                                                        //printf("%d: %s\n", lineCount, nextString);
                                                        WordCount++;
                                                    }
                                                    */
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            //cleanup
                            fclose(fp);
                        }
                    }
                    closedir(innerdir);
                }
            }
        }
        closedir(dir);
    }

    printf("WordCount: %d\n", WordCount);

    printf("**Loading Complete\n");

    //for testing
    /*
    for (int i = 0; i < WordCount; i++)
    {
        printf("%s\n", WordArray[i]);
    }
    */

    //sorting if necessary
    if (sortFlag == 1)
    {
        printf("Sorting...\n");

        //shell sort
        char **SortedWordArray = sortingAlphabeticallyFunction(WordArray, WordCount);

        
        int sortingCheck = AfterSort(SortedWordArray, WordCount, Wfp);

        if (sortingCheck == -1)
        {
            printf("Error: problem while sorting\n");
            free(WordArray);
            fclose(Wfp);
            return -1;
        }
        
        printf("**Sorting Completed\n");
    }

    //cleanup
    fclose(Wfp);
    for (int i = 0; i < WordCount - 1 ;i++)
    {
        free(WordArray[i]);
        WordArray[i] = NULL;
    }
    free(WordArray);
    WordArray = NULL;

    //confirm that the file was generated successfully
    printf("***File generated sucessfully***\n");

    clock_t TimeDif = clock() - Start;
    double TimeTaken = (double)TimeDif / CLOCKS_PER_SEC;

    printf("Operation took: %f Seconds to complete\n\n", TimeTaken);

    //user input (just to confirm that the user can return back to the previous menu)
    char ActionChoice2[128];
    printf("Type anything to return: ");
    scanf("%s", ActionChoice2);

    return 0;
}

//function used to sort a list of words in aplphabetical order
//using a shell sort
char** sortingAlphabeticallyFunction(char **WordArray, int WordCount)
{
    printf("%d: words to sort\n", WordCount);

    MergeSort(WordArray, 0, WordCount - 1, WordCount);

    return WordArray;
}

void MergeSort(char**WordArray, int LeftValue, int RightValue, int WordCount)
{
    if (LeftValue < RightValue)
    {
        int MiddleValue = (LeftValue + (RightValue - 1)) / 2;

        MergeSort(WordArray, LeftValue, MiddleValue, WordCount);
        MergeSort(WordArray, MiddleValue + 1, RightValue, WordCount);
        Merge(WordArray, LeftValue, MiddleValue, RightValue, WordCount);
    }
}

void Merge(char**WordArray, int LeftValue, int MiddleValue, int RightValue, int WordCount)
{
    int LeftArrayRange = MiddleValue - LeftValue + 1;
    int RightArrayRange = RightValue - MiddleValue;

    char**LeftArray = malloc(LeftArrayRange * sizeof(char*));
    char**RightArray = malloc(RightArrayRange * sizeof(char*));

    for (int i = 0; i < LeftArrayRange; i++)
    {
        //printf("IValue: %d, LeftValue + i: %d, WordCount: %d\n", i, LeftValue+i, WordCount);
        LeftArray[i] = malloc(25*sizeof(char));
        strcpy(LeftArray[i], WordArray[LeftValue + i]);
    }
    for (int j = 0; j < RightArrayRange; j++)
    {
        //printf("JValue: %d, MiddleValue + 1 + j: %d, WordCount: %d\n", j, MiddleValue + 1 + j, WordCount);
        RightArray[j] = malloc(25*sizeof(char));
        strcpy(RightArray[j], WordArray[MiddleValue + 1 + j]);
    }

    int i = 0, j = 0, k = LeftValue;

    while( i < LeftArrayRange && j < RightArrayRange)
    {
        //printf("IValue: %d, JValue: %d, KValue: %d, WordCount: %d\n", i, j, k, WordCount);
        if( strcmp(LeftArray[i], RightArray[j]) < 0)
        {
            strcpy(WordArray[k], LeftArray[i]);
            i++;
        }
        else
        {
            strcpy(WordArray[k], RightArray[j]);
            j++;
        }
        k++;
    }

    while (i < LeftArrayRange)
    {
        //printf("IValue: %d, KValue: %d, WordCount: %d\n", i, k, WordCount);
        strcpy(WordArray[k], LeftArray[i]);
        i++;
        k++;
    }

    while (j < RightArrayRange)
    {
        //printf("JValue: %d, KValue: %d, WordCount: %d\n", j, k, WordCount);
        strcpy(WordArray[k], RightArray[j]);
        j++;
        k++;
    }
}

int AfterSort(char** WordArray, int WordCount, FILE* Wfp)
{
    //loop to add sorted list to final file
    for (int i = 0; i < WordCount - 1; i++)
    {
        fprintf(Wfp, "%s\n", WordArray[i]);
        //printf("%d: %s\n", i, WordArray[i]);
    }

    return 0;
}