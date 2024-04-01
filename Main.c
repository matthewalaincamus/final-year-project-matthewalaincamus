#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#include "ListGenerator.h"
#include "SpellChecker.h"

//list of functions:
int mainMenu();
int GenerateDataset();
int TypeSelector();
int SpellCheck(int FileType);

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
        printf("(B) perform spell checking\n");
        printf("(Z) Exit system\n");
        printf("\n");

        //check to see if the user wishes to close the system or not
        int inputCheck = 1;

        while(inputCheck == 1)
            {
            //user input
            char ActionChoice[128];
            printf(": ");
            scanf("%s", ActionChoice);

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
            else if(!strcmp(ActionChoice, "B") || !strcmp(ActionChoice, "b"))
            {
                int errorCheck = TypeSelector();

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
                
                //remove all files to save storage space
                remove("./corpus/LWD.txt");
                remove("./corpus/LWDS.txt");
                remove("./corpus/LWND.txt");
                remove("./corpus/LWNDS.txt");
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
        printf("(B) text into single list without duplicates\n");
        printf("(C) text into single list with duplicates and sorted alphabeticaly\n");
        printf("(D) text into single list without duplicates and sorted alphabeticaly (merge sort)\n");
        printf("(Z) return to main menu\n");

        //check to see if the user wishes to close the system or not
        int inputCheck = 1;

        while(inputCheck == 1)
        {
            //user input
            char ActionChoice[128];
            printf(": ");
            scanf("%s", ActionChoice);

            if(!strcmp(ActionChoice, "A") || !strcmp(ActionChoice, "a"))
            {
                int errorCheck = DataReader(1, 0);

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
            else if(!strcmp(ActionChoice, "B") || !strcmp(ActionChoice, "b"))
            {
                int errorCheck = DataReader(0, 0);

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
            else if(!strcmp(ActionChoice, "C") || !strcmp(ActionChoice, "c"))
            {
                int errorCheck = DataReader(1, 1);

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
            else if(!strcmp(ActionChoice, "D") || !strcmp(ActionChoice, "d"))
            {
                int errorCheck = DataReader(0, 1);

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

int TypeSelector()
{
    system("cls");

    int FileFlags[4] = {0, 0, 0, 0};

    struct dirent *dp;
    char directoryName[50] = "./corpus/";
    
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
            if (!strcmp(dp->d_name,"LWD.txt"))
            {
                FileFlags[0] = 1;
            }
            else if (!strcmp(dp->d_name,"LWND.txt"))
            {
                FileFlags[1] = 1;
            }
            else if (!strcmp(dp->d_name,"LWDS.txt"))
            {
                FileFlags[2] = 1;
            }
            else if (!strcmp(dp->d_name,"LWNDS.txt"))
            {
                FileFlags[3] = 1;
            }
        }
    }
    closedir(dir);

    if(FileFlags[0] == 0 && FileFlags[1] == 0 && FileFlags[2] == 0 && FileFlags[3] == 0)
    {
        printf("You will need to generate a word file from the corpus before spell checking is possible\n");

        //user input
        char ActionChoice[128];
        printf("Press anything to return to the main menu: ");
        scanf("%s", ActionChoice);

        return 0;
    }
    else
    {
        printf("Please select a wordlist from the options below (type the respective letter)\n");

        if (FileFlags[0] == 1)
        {
            printf("(A) use text with duplicate\n");
        }
        if (FileFlags[1] == 1)
        {
            printf("(B) use text without duplicates\n");
        }
        if (FileFlags[2] == 1)
        {
            printf("(C) use text with duplicates sorted in alphabetical order\n");
        }
        if (FileFlags[3] == 1)
        {
            printf("(D) use text without duplicates sorted in alphabetical order\n");
        }

        printf("(Z) return to main menu\n");

        int inputCheck = 1;

        while(inputCheck == 1)
        {
            //user input
            char ActionChoice[128];
            printf(": ");
            scanf("%s", ActionChoice);

            if((!strcmp(ActionChoice, "A") || !strcmp(ActionChoice, "a")) && FileFlags[0] == 1)
            {
                SpellCheck(0);
                inputCheck = 0;
            }
            else if((!strcmp(ActionChoice, "B") || !strcmp(ActionChoice, "b")) && FileFlags[1] == 1)
            {
                SpellCheck(1);
                inputCheck = 0;
            }
            else if((!strcmp(ActionChoice, "C") || !strcmp(ActionChoice, "c")) && FileFlags[2] == 1)
            {
                SpellCheck(2);
                inputCheck = 0;
            }
            else if((!strcmp(ActionChoice, "D") || !strcmp(ActionChoice, "d")) && FileFlags[3] == 1)
            {
                SpellCheck(3);
                inputCheck = 0;
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
    }

    return 0;
}

int SpellCheck(int FileType)
{
    //check to see if the user wants to return to the main menu
    int returnCheck = 1;

    //will loop till the user exits via typing z or Z on the keyboard in the main menu
    while (returnCheck == 1)
    {
        //clear the screen so that only relevant system text is shown
        system("cls");

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

        //show user options for the corpus
        printf("options for spell checking (indicate which option by typing its asscociated letter):\n");
        printf("(A) simple check using linear search\n");
        printf("(B) simple check binary search (sorted word list only)\n");
        printf("(C) simple check using linear search, followed by suggestions if wrong using Levenshtein Distance\n");
        printf("(D) simple check using binary search (sorted word list only), followed by suggestions if wrong using Levenshtien Distance\n");
        printf("(E) simple check using linear search, followed by suggestions if wrong using Hamming Distance\n");
        printf("(F) simple check using binary search (sorted word list only), followed by suggestions if wrong using Hamming Distance\n");
        printf("(G) simple check using linear search, followed by suggestions if wrong using Sorensen-Dice Coefficient\n");
        printf("(H) simple check using binary search (sorted word list only), followed by suggestions if wrong using Sorensen-Dice Coefficient\n");
        printf("(I) simple check using linear search, followed by suggestions if wrong using Optimal String Alignment Distance\n");
        printf("(J) simple check using binary search (sorted word list only), followed by suggestions if wrong using Optimal String Alignment Distance\n");
        printf("(Z) return to main menu\n");

        
        int inputCheck = 1;

        while(inputCheck == 1)
        {
            //user input
            char ActionChoice[128];
            printf(": ");
            scanf("%s", ActionChoice);

            if(!strcmp(ActionChoice, "A") || !strcmp(ActionChoice, "a"))
            {
                
                int errorCheck = SpellChecker(FileType, 0, 0);

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
            else if((!strcmp(ActionChoice, "B") || !strcmp(ActionChoice, "b")) && ((FileType == 2) || (FileType == 3)))
            {
                
                int errorCheck = SpellChecker(FileType, 1, 0);

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
            else if(!strcmp(ActionChoice, "C") || !strcmp(ActionChoice, "c"))
            {
                
                int errorCheck = SpellChecker(FileType, 0, 1);

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
            else if((!strcmp(ActionChoice, "D") || !strcmp(ActionChoice, "d")) && ((FileType == 2) || (FileType == 3)))
            {
                
                int errorCheck = SpellChecker(FileType, 1, 1);

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
            else if(!strcmp(ActionChoice, "E") || !strcmp(ActionChoice, "e"))
            {
                
                int errorCheck = SpellChecker(FileType, 0, 2);

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
            else if((!strcmp(ActionChoice, "F") || !strcmp(ActionChoice, "f")) && ((FileType == 2) || (FileType == 3)))
            {
                
                int errorCheck = SpellChecker(FileType, 1, 2);

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
            else if(!strcmp(ActionChoice, "G") || !strcmp(ActionChoice, "g"))
            {
                
                int errorCheck = SpellChecker(FileType, 0, 3);

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
            else if((!strcmp(ActionChoice, "H") || !strcmp(ActionChoice, "h")) && ((FileType == 2) || (FileType == 3)))
            {
                
                int errorCheck = SpellChecker(FileType, 1, 3);

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
            else if(!strcmp(ActionChoice, "I") || !strcmp(ActionChoice, "i"))
            {
                
                int errorCheck = SpellChecker(FileType, 0, 4);

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
            else if((!strcmp(ActionChoice, "J") || !strcmp(ActionChoice, "j")) && ((FileType == 2) || (FileType == 3)))
            {
                
                int errorCheck = SpellChecker(FileType, 1, 4);

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
            //if user attempts to use a binary search with a non sorted file type, reject the request
            else if((!strcmp(ActionChoice, "B") ||
                !strcmp(ActionChoice, "b") ||
                !strcmp(ActionChoice, "D") || 
                !strcmp(ActionChoice, "d") ||
                !strcmp(ActionChoice, "F") || 
                !strcmp(ActionChoice, "f") ||
                !strcmp(ActionChoice, "H") || 
                !strcmp(ActionChoice, "h") ||
                !strcmp(ActionChoice, "J") || 
                !strcmp(ActionChoice, "j")) && 
                ((FileType == 0) || (FileType == 1)))
            {
                printf("the selected file type needs to be sorted to perform this action\n");
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