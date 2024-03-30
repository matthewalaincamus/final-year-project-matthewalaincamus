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

//spell checking function
int SpellChecker(int FileType, int CheckType, int LevCheck);

//linear searcher
int linearCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, clock_t Start);

//binary seacher
int binaryCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, clock_t Start);

//function to get words from file
struct WordsListing WordFetcher(int FileType);

//recursive binary search
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[25]);

//levenshtein algorithm
int LevenshteinChecker(char string1[], char string2[], int len1, int len2);

//hamming algorithm
int HammingChecker(char string1[], char string2[], int len1, int len2);

//min function used in levenshtein checker
int Min(int a, int b);