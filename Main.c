#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//list of functions:
int mainMenu();
int GenerateDataset();

//dataset generation functions
int LWD_Dataset();

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

    printf("***Generating list***\n");

    FILE* fp;
    char nextString[2048];

    fp = fopen("./corpus/Texts/A/A0/A00.xml", "r");

    if (fp == NULL)
    {
        printf("Error: unknown xml file\n");
        return -1;
    }
    else
    {
        while(!feof(fp))
        {
            fgets(nextString, 2047, fp);
            printf("%s", nextString);
        }
    }

    //cleanup
    fclose(fp);

    //confirm that the file was generated successfully
    printf("***File generated sucessfully***\n");

    //user input (just to confirm that the user can return back to the previous menu)
    char ActionChoice[128];
    printf("Type anything to return: ");
    scanf("%s", &ActionChoice);

    return 0;
}