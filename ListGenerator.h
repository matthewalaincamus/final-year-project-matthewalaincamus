#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

//list generation functions
int DataReader(int duplicateFlag, int sortFlag);

//sorting algorithms
char** sortingAlphabeticallyFunction(char **WordArray, int WordCount);
void MergeSort(char**WordArray, int LeftValue, int RightValue, int WordCount);
void Merge(char**WordArray, int LeftValue, int MiddleValue, int RightValue, int WordCount);
int AfterSort(char** WordArray, int WordCount, FILE* Wfp);