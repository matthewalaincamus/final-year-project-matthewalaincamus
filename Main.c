#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

//list of functions:
int mainMenu();
int GenerateDataset();

//dataset generation functions
int LWD_Dataset();
int wordReader(char fileLocation[128], FILE* Wfp);

//main program to deal with what the user wishes to do with the system
int main()
{
    //got to the main menu to choose what function to use
    mainMenu();

    return 0;
}

int mainMenu()
{
    //check to see if the user wants to exist the system
    int returnCheck = 1;

    //will loop till the user exits via typing z or Z on the keyboard in the main menu
    while (returnCheck == 1)
    {
        //clear the screen so that only relevant system text is shown
        system("cls");

        //introduce the system
        printf("***Welcome to Matthew Camus's spell checking software***\n");
        printf("\n");

        //show user options that they can do
        printf("options to choose from (indicate which option by typing its asscociated letter):\n");
        printf("(A) Generate a dataset from the Corpus\n");
        printf("(Z) Exit system\n");
        printf("\n");

        //check to see if the user wishes to close the system or not
        int inputCheck = 1;

        while(inputCheck == 1)
            {
            //user input
            char ActionChoice[128];
            printf(": ");
            scanf("%s", &ActionChoice);

            if(!strcmp(ActionChoice, "A") || !strcmp(ActionChoice, "a"))
            {
                int errorCheck = GenerateDataset();

                //if -1, an error occured.
                //if 0, then reload the page again
                if (errorCheck == 0)
                {
                    inputCheck = 2;
                }
                if (errorCheck == -1)
                {
                    inputCheck = -1;
                }
                
            }
            else if(!strcmp(ActionChoice, "Z") || !strcmp(ActionChoice, "z"))
            {
                inputCheck = 0;
            }
            //if the user types an invalid answer
            else
            {
                printf("Invalid response, please try again\n");
            }
        }
        // if inputCheck == 0 that means that the user wishes to leave the system
        // otherwise it means that the user still wishes to do other things with the system or and error has occured (system will loop)
        if (inputCheck == 0)
        {
            returnCheck = 0;
        }
        else if (inputCheck == -1)
        {
            returnCheck = -1;
        }
    }

    return 0;
}

/***
 * All dataset generation functions
*/

//function for dealing with all things regarding the generation of a file of words for use in the spell checking functions
int GenerateDataset()
{

    //check to see if the user wants to return to the main menu
    int returnCheck = 1;

    //will loop till the user exits via typing z or Z on the keyboard in the main menu
    while (returnCheck == 1)
    {
        //clear the screen so that only relevant system text is shown
        system("cls");

        //show user options for the corpus
        printf("options for corpus (indicate which option by typing its asscociated letter):\n");
        printf("(A) text into single list with duplicates\n");
        printf("(Z) return to main menu\n");

        //check to see if the user wishes to close the system or not
        int inputCheck = 1;

        while(inputCheck == 1)
        {
            //user input
            char ActionChoice[128];
            printf(": ");
            scanf("%s", &ActionChoice);

            if(!strcmp(ActionChoice, "A") || !strcmp(ActionChoice, "a"))
            {
                int errorCheck = LWD_Dataset();

                //if -1, an error occured.
                //if 0, then reload the page again
                if (errorCheck == 0)
                {
                    inputCheck = 2;
                }
                else if (errorCheck == -1)
                {
                    inputCheck = -1;
                }
            }
            else if(!strcmp(ActionChoice, "Z") || !strcmp(ActionChoice, "z"))
            {
                inputCheck = 0;
            }
            //if the user types an invalid answer
            else
            {
                printf("Invalid response, please try again\n");
            }
        }
        // if inputCheck == 0 that means that the user wishes to leave the system
        // otherwise it means that the user still wishes to do other things with the system or and error has occured (system will loop)
        if (inputCheck == 0)
        {
            returnCheck = 0;
        }
        else if(inputCheck == -1)
        {
            returnCheck = -1;
        }
    }
    
    return returnCheck;
}

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
                            printf("%s\n", FinalDirectoryName);
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

}