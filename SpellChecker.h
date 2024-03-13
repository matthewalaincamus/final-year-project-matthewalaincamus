#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>

struct WordsListing 
{
    char** WordArray;
    int WordCount;
};

int simpleCheckLinear(int FileType, int LevCheck);
int simpleCheckBinary(int FileType);
struct WordsListing WordFetcher(int FileType);
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[25]);
int LevenshteinChecker(char string1[], char string2[], int len1, int len2);
int Min(int a, int b);