#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

//list generation functions
int DataReader(int duplicateFlag, int sortFlag);

//sorting algorithms
char** sortingAlphabeticallyFunction(FILE* Wfp, int StartValue, int MidValue, int EndValue);
int AfterSort(char** WordArray, int WordCount, FILE* Wfp);