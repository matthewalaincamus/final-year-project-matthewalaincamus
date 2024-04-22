#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(){
    
    //get the words from the file and open a pointer to it
    FILE *fptr;
    fptr = fopen("./words_data/words_alpha.txt","r");

    if(fptr == NULL){
        printf("error in opening file");
        fclose(fptr);
        return 1;
    }
    
    char nextChar;

    //make a string array to store the words
    char **WordArray;
    WordArray = malloc(370100 * sizeof(char*));

    int index = 0;

    while (nextChar != EOF){
        //get next char in file
        nextChar = fgetc(fptr);
        
        if (nextChar == '#'){
            while(nextChar != '\n'){
                nextChar = fgetc(fptr);
            }
        }

        else{
            char testString[128];
            int i = 0;

            while(nextChar != '\n' && nextChar != EOF){
                //printf("[%i]:[%i]\n", index, i);
                testString[i] = nextChar;
                i++;
                nextChar = fgetc(fptr);
            }

            //just to prevent segmentation upon reaching the last word
            if (nextChar == EOF){
                ungetc(nextChar, fptr);
            }

            testString[i] = '\0';

            //printf("%s\n", testString);
            WordArray[index] = malloc(128 * sizeof(char));
            strcpy(WordArray[index], testString);
            index++;
        }
    }
    //printf("%s\n", WordArray[0]);
    //printf("%s\n", WordArray[370098]);

    //user input
    char userWord[128];
    printf("Enter a word you want to check: ");
    scanf("%s", userWord);

    //start timer
    struct timeval Start;
    gettimeofday(&Start, 0);

    //check to see if the word is in the list
    int WordFoundFlag = 0;
    for (int i = 0; i < 307100; i++){
        if (!strcmp(WordArray[i], userWord)){
            WordFoundFlag = 1;
            break;
        }
    }

    //confirm if the word is in the list or not
    if (WordFoundFlag == 1){
        printf("word is in the dictionary\n");
    }
    else{
        printf("word isn't in the dictionary\n");
    }

    //get the time of completion
    struct timeval Stop;
    gettimeofday(&Stop, 0);

    float TimeTaken = (Stop.tv_sec - Start.tv_sec) * 1000.f + (Stop.tv_usec - Start.tv_usec) / 1000.f;

    printf("Operation took: %f miliseconds to complete\n\n", TimeTaken);

    //cleanup
    free(WordArray);
    fclose(fptr);
    
    return 0;
}