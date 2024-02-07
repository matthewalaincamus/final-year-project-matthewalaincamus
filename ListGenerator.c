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
        scanf("%s", &ActionChoice1);

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

    printf("***Generating list***\n");
    printf("LOADING...\n");

    //make a string array to store the words for duplicate checking
    char **WordArray;
    WordArray = malloc(20000000 * sizeof(char*));
    int WordCount = 0;

    FILE* Wfp;

    //this is the file where all the words are deposited
    if (sortFlag == 0 && duplicateFlag == 1)Wfp = fopen("./corpus/LWD.txt", "w");
    else if (sortFlag == 1 && duplicateFlag == 1)Wfp = fopen("./corpus/LWDS_tmp.txt", "w");
    else if (sortFlag == 0 && duplicateFlag == 0)Wfp = fopen("./corpus/LWND.txt", "w");
    else if (sortFlag == 1 && duplicateFlag == 0)Wfp = fopen("./corpus/LWNDS_tmp.txt", "w");
    else
    {
        printf("Error: invalid sortCheck or duplicateCheck value given\n");
        free(WordArray);
        fclose(Wfp);
        return -1;
    }

    if (Wfp == NULL)
    {
        printf("Error: unable to open word file\n");
        free(WordArray);
        fclose(Wfp);
        return -1;
    }

    struct dirent *dp;
    char directoryName[128] = "./corpus/Texts/";
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
                char innnerDirectoryName[128];
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
                            char FinalDirectoryName[128];
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
                                free(WordArray);
                                fclose(Wfp);
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

                                                            if(duplicateCheck == 0 & duplicateFlag == 0)
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
                                                            else if (duplicateFlag == 1)
                                                            {
                                                                fprintf(Wfp, "%s\n", nextString);
                                                                //printf("%d: %s\n", lineCount, nextString);
                                                                WordCount++;
                                                            }
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

                                                    if(duplicateCheck == 0 & duplicateFlag == 0)
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
                                                    else if (duplicateFlag == 1)
                                                    {
                                                        fprintf(Wfp, "%s\n", nextString);
                                                        //printf("%d: %s\n", lineCount, nextString);
                                                        WordCount++;
                                                    }
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

    printf("**Loading Complete\n");

    //for testing
    /*
    for (int i = 0; i < WordCount; i++)
    {
        printf("%s\n", WordArray[i]);
    }
    */

    //cleanup
    fclose(Wfp);
    free(WordArray);

    //sorting if necessary
    if (sortFlag == 1)
    {
        printf("Sorting...\n");

        FILE *fp, *Wfp;

        if (duplicateFlag == 1)
        {
            fp = fopen("./corpus/LWDS_tmp.txt", "r");
            Wfp = fopen("./corpus/LWDS.txt", "w");
        }
        else if (duplicateFlag == 0)
        {
            fp = fopen("./corpus/LWNDS_tmp.txt", "r");
            Wfp = fopen("./corpus/LWNDS.txt", "w");
        }

        if (fp == NULL)
        {
            printf("Error: unable to open sorting temporary word file\n");
            fclose(fp);
            return -1;
        }

        if (Wfp == NULL)
        {
            printf("Error: unable to open final sorted word file\n");
            fclose(Wfp);
            return -1;
        }

        //merge sort
        //char **SortedWordArray = sortingAlphabeticallyFunction(Wfp, 0, WordCount/2, WordCount);

        /*
        int sortingCheck = AfterSort(WordArray, WordCount, Wfp);

        if (sortingCheck == -1)
        {
            printf("Error: problem while sorting\n");
            free(WordArray);
            fclose(Wfp);
            return -1;
        }
        */

        printf("Sorting Completed\n");

        //cleanup
        fclose(Wfp);
        fclose(fp);
    }

    //confirm that the file was generated successfully
    printf("***File generated sucessfully***\n");

    //user input (just to confirm that the user can return back to the previous menu)
    char ActionChoice2[128];
    printf("Type anything to return: ");
    scanf("%s", &ActionChoice2);

    return 0;
}

//function used to sort a list of words in aplphabetical order
//using a merge sort since I am dealing with numbers of words in the order of millions
char** sortingAlphabeticallyFunction(FILE* Wfp, int StartValue, int MidValue, int EndValue)
{
    int i, j, k;

    int LeftNumber = MidValue - StartValue + 1;
    int RightNumber = EndValue - MidValue;

    char** LeftArray = malloc((LeftNumber) * sizeof(char*));
    char** RightArray = malloc((RightNumber) * sizeof(char*));

    /*
    for (i = 0; i < LeftNumber; i++)
    {
        LeftArray[i] = malloc(25 * sizeof(char));
        strcpy(LeftArray[i], WordArray[i]);
    }
    for (j = 0; j < RightNumber; j++)
    {
        RightArray[j] = malloc(25 * sizeof(char));
        strcpy(RightArray[j], WordArray[j + MidValue + 1]);
    }
    */

    /*
    //this is the loop to sort a string list in alphabetical order
    for (int i = 0; i < WordCount; i++)
    {
        for (int j = i + 1; j < WordCount; j++)
        {
            if(strcmp(WordArray[i],WordArray[j]) > 0)
            {
                char *tmp = malloc(50 * sizeof(char));
                strcpy(tmp,WordArray[i]);
                strcpy(WordArray[i], WordArray[j]);
                strcpy(WordArray[j], tmp);
                
                free(tmp);
            }
        }
    }
    */
   free(LeftArray);
   free(RightArray);

    return 0;
}

int AfterSort(char** WordArray, int WordCount, FILE* Wfp)
{
    //loop to add sorted list to final file
    for (int i = 0; i < WordCount; i++)
    {
        fprintf(Wfp, "%s\n", WordArray[i]);
        //printf("%d: %s\n", i, WordArray[i]);
    }

    return 0;
}