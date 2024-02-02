#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

//list generation functions
int LWD_Dataset();
int wordReader(char fileLocation[128], FILE* Wfp);

int LWND_Dataset();