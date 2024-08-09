/* heaedr cment
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int kNumBuckets = 127;
const int kMaxNameLength = 21;
const int kMaxLineLength = 40;
const char FILENAME[15] = "countries.txt";

struct ParcelNode {
    char* destination;
    int weight;
    float value;
    struct ParcelNode* left;
    struct ParcelNode* right;
};

struct Tree {
    struct ParcelNode root;
};

void replaceChar(char* input, char oldc, char newc);
unsigned long hash(char* str);
void insertIntoHashtable(struct Tree* hashtable[], char* destination, int weight, float value);
struct ParcelNode* createNode(char* destination, int weight, float value);
void insertIntoTree(struct Tree* root, ParcelNode* newNode);

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

/*
* TITLE : djb2 hash function
* AUTHOR : Dan Bernstein
* DATE : 2024-07-16
* VERSION : 2?
* AVAILABIILTY : from lecture Week 9 - Hash Tables
*/
/*
* FUNCTION: hash
* DESCRIPTION: gets hash value for a string.
* PARAMETERS: char* str: string to get hash for
* RETURNS: unsigned long hash: hash value
*/
unsigned long hash(char* str) {
    unsigned long hash = 5381;
    int c = 0;
    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}



//comment
void insertIntoHashtable(Tree* hashtable[], char* destination, int weight, float value) {
    int hashIndex = hash(destination) % kNumBuckets;
    struct ParcelNode* newNode = createNode(destination, weight, value);
    insertIntoTree(hashtable[hashIndex], newNode);
}


/*
* FUNCTION: createNode
* DESCRIPTION: create new ParcelNode with provided info
* PARAMETERS: char* country: destination of parcel
*             int weight: weight of parcel in g
*             float value: value of parcel in $
* RETURNS: struct ParcelNode* newNode: pointer to new node
*/
struct ParcelNode* createNode(char* country, int weight, float value) {
    struct ParcelNode* newNode = (struct ParcelNode*)malloc(sizeof(struct ParcelNode));
    if (newNode == NULL) {
        return NULL;
    }
    int nameLength = (int)strlen(country);
    newNode->destination = (char*)malloc(nameLength + 1);
    if (newNode->destination) {
        strcpy(newNode->destination, country);
    }
    newNode->weight = weight;
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//comment
void insertIntoTree(Tree* root, ParcelNode* newNode) {
    if (!root) { //root is NULL
        //put node as root
    }
    else {
        //insert by weight
    }
}

void insertIntoTree_rec(ParcelNode parent, ParcelNode* newNode) {

}


