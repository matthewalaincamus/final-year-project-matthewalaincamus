#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/timeb.h>
#include <math.h>

//for single word array
struct WordsListing 
{
    char** WordArray;
    int WordCount;
};

//for returning values for automated testing
struct TimeAndSuggestion
{
    float TimeTaken;
    int SuggestionNumber;
};

//for processing mispelled words in the auto tester
struct MisspelledWord
{
    char Word[50];
    int index;
};

//hash table node
struct WordNode
{
    int key;
    char Word[50];
    int AlgorithmDistance;
    struct WordNode *next;
};

//hash table linked list
struct WordHashItem
{
    struct WordNode *head;
    struct WordNode *tail;
};

//hash table return struct
struct ReturnHash
{
    struct WordHashItem *HashTable;
    int WordCount;
};



//spell checking function
int SpellChecker(int FileType, int CheckType, int AlgorithmCheck);

//linear searcher
struct TimeAndSuggestion linearCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString);

//binary seacher
struct TimeAndSuggestion binaryCheck(char ** WordArray, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString);

//hash table searcher
struct TimeAndSuggestion HashChecker(struct WordHashItem *HashTable, int MaxSize, int WordCount, char wordString[], int AlgorithmCheck, int TestFlag, char TestString);

//function to get words from file
struct WordsListing WordFetcher(int FileType);

//function to make hash from file data
struct ReturnHash initHash(int FileType, int MaxSize);

//recursive binary search
int binarySearch(char** WordArray, int leftValue, int rightValue, char wordString[]);

//levenshtein algorithm
int LevenshteinDistance(char string1[], char string2[], int len1, int len2);

//hamming algorithm
int HammingDistance(char string1[], char string2[], int len1, int len2);

//Sørensen-Dice Coefficient algorithm
int SorensenDiceCoefficient(char string1[], char string2[], int len1, int len2);

//Optimal String Alignment Distance algorithm
int OptimalStringAlignmentDistance(char string1[], char string2[], int len1, int len2);

//Damerau-Levenshtein Distance algorithm
int DamerauLevenshteinDistance(char string1[], char string2[], int len1, int len2);

//Jaro Similarity algorithm
int JaroSimilarity(char string1[], char string2[], int len1, int len2);

//Jaro-Winkler Similarity algorithm
int JaroWinklerSimilarity(char string1[], char string2[], int len1, int len2);

//max function used in Jaro Similarity algorithm
int Max(int a, int b);

//min function used in levenshtein checker
int Min(int a, int b);

//min for 3 number function used in Optimal String Alignment Distance algorithm
int Min3(int a, int b, int c);

//function to handle full testing and comparison of methods and algorithms
int AutoSpellChecker(int FileType);

//function to compare the algorithms against the mispelled data
int AlgorithmAssessor(int FileType);