/* heaedr cment
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int kNumBuckets = 127;
const int kMaxNameLength = 21;
const int kMaxLineLength = 40;
const char FILENAME[15] = "countries.txt";

struct Parcel {
    char* destination;
    int weight;
    float value;
};

struct TreeNode {
    struct Parcel data;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct Tree {
    struct TreeNode root;
};

void replaceChar(char* input, char oldc, char newc);
void insertIntoHashtable(struct Tree* hashtable[], char* destination, int weight, float value);


int main(void) {
    struct Tree* hashtable[kNumBuckets] = {};

    FILE* pParcelsFile = NULL;
    if ((pParcelsFile = fopen(FILENAME, "r")) == NULL) {
        printf("File I/O error\n");
        return 1;
    }

    char parcelBuffer[kMaxLineLength] = "";

    while (!feof(pParcelsFile) and !ferror(pParcelsFile)) {
        if (fgets(parcelBuffer, kMaxLineLength, pParcelsFile)) {
            replaceChar(parcelBuffer, '\n', '\0');
            char countryName[kMaxNameLength] = "";
            int weight = 0;
            float value = 0;
            if (sscanf(parcelBuffer, "%[^,], %d, %f", countryName, &weight, &value) != 3) {
                //line format incorrect
                continue;
            }
            //insert in hashtable of trees
            insertIntoHashtable(hashtable, countryName, weight, value);
        }
        else {
            printf("fgets error\n");
        }
    }

    if (fclose(pParcelsFile) != 0) {
        printf("File I/O error\n");
        return 1;
    }

}


//comment
void insertIntoHashtable(Tree* hashtable[], char* destination, int weight, float value) {
}


/*
* FUNCTION: replaceChar
* DESCRIPTION: takes a string and replaces the first instance of char oldc with char newc.
*              does nothing if oldc not found.
* PARAMETERS: char* input: input string
*             char oldc: char to replace
*             char newc: char to put in
* RETURNS: none
*/
void replaceChar(char* input, char oldc, char newc) {
    char* pNewc = strchr(input, oldc);
    if (pNewc) {
        *pNewc = newc;
    }
}


