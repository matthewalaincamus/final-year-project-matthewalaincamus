#include "ListGenerator.h"

//function for making list of words with no checks for duplicates
int LWD_Dataset()
{
    //clear the screen so that only relevant system text is shown
    system("cls");

    //show user options for the corpus
    printf("Please type a letter from A to K as two which text you would like (has to be capitals):\n");

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

    FILE* Wfp;

    //this is the file where all the words are deposited
    Wfp = fopen("./corpus/words.txt", "w");

    if (Wfp == NULL)
    {
        printf("Error: unable to open word file\n");
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
                            //printf("%s\n", FinalDirectoryName);

                            int ReaderCheck =  wordReader(FinalDirectoryName, Wfp);
                            if (ReaderCheck == -1) return -1;
                        }
                    }
                    closedir(innerdir);
                }
            }
        }
        closedir(dir);
    }

    
    fclose(Wfp);

    //confirm that the file was generated successfully
    printf("***File generated sucessfully***\n");

    //user input (just to confirm that the user can return back to the previous menu)
    char ActionChoice2[128];
    printf("Type anything to return: ");
    scanf("%s", &ActionChoice2);

    return 0;
}

int wordReader(char fileLocation[128], FILE* Wfp)
{
    FILE* fp;
    char nextChar;

    fp = fopen(fileLocation, "r");

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
                        char nextString[128];

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
                            nextString[charCount] = nextChar;
                            nextChar = fgetc(fp);
                            charCount++;
                        }
                        nextString[charCount] = '\0';
                        //word shouldn't have numbers or start on anything other than a letter
                        //if a word is valid, it is added to the final file of words
                        if (breakCheck == 0 && isalpha(nextString[0]))
                        {
                            fprintf(Wfp, "%s\n", nextString);
                            //printf("%d: %s\n", lineCount, nextString);
                        }
                    }
                }
            }
        }
        //printf("%d\n", lineCount);
    }

    //cleanup
    fclose(fp);

    return 0;

}

//function for making list of words with no checks for duplicates
int LWND_Dataset()
{
    //clear the screen so that only relevant system text is shown
    system("cls");

    //show user options for the corpus
    printf("Please type a letter from A to K as two which text you would like (has to be capitals):\n");

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

     //make a string array to store the words for duplicate checking
    char **WordArray;
    WordArray = malloc(1000000 * sizeof(char*));
    int WordCount = 0;

    FILE* Wfp;

    //this is the file where all the words are deposited
    Wfp = fopen("./corpus/words.txt", "w");

    if (Wfp == NULL)
    {
        printf("Error: unable to open word file\n");
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
                            //printf("%s\n", FinalDirectoryName);

                            FILE* fp;
                            char nextChar;

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
                                                char nextString[128];

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
                                                    nextString[charCount] = nextChar;
                                                    nextChar = fgetc(fp);
                                                    charCount++;
                                                }
                                                nextString[charCount] = '\0';
                                                //word shouldn't have numbers or start on anything other than a letter
                                                //if a word is valid, it is added to the final file of words
                                                if (breakCheck == 0 && isalpha(nextString[0]))
                                                {
                                                    int duplicateCheck = 0;

                                                    for (int i = 0; i < WordCount; i++)
                                                    {
                                                        if (!strcmp(WordArray[i], nextString))
                                                        {
                                                            duplicateCheck = 1;
                                                            break;
                                                        }
                                                    }
                                                    if(duplicateCheck == 0)
                                                    {
                                                        WordArray[WordCount] = malloc(128 * sizeof(char));
                                                        strcpy(WordArray[WordCount], nextString);
                                                        WordCount++;

                                                        fprintf(Wfp, "%s\n", nextString);
                                                        //printf("%d: %s\n", lineCount, nextString);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                //printf("%d\n", lineCount);
                            }
                            //cleanup
                            fclose(fp);

                            //for testing
                            break;

                        }
                    }
                    closedir(innerdir);
                }
            }
        }
        closedir(dir);
    }

    //for testing
    /*
    for (int i = 0; i < WordCount; i++)
    {
        printf("%s\n", WordArray[i]);
    }
    */
    
    fclose(Wfp);
    free(WordArray);

    //confirm that the file was generated successfully
    printf("***File generated sucessfully***\n");

    //user input (just to confirm that the user can return back to the previous menu)
    char ActionChoice2[128];
    printf("Type anything to return: ");
    scanf("%s", &ActionChoice2);

    return 0;
}