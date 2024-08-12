/*
* FILE : dsfinal.cpp
* PROJECT : SENG1050 - Final Project
* PROGRAMMER : Jason Shu
* FIRST VERSION : 2024-08-12
* DESCRIPTION :
* This is a program to compute and show various statistics and info about parcels, 
*  using hashtables and trees.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string> //delete later
#pragma warning(disable: 4996)

const int kNumBuckets = 127;
const int kMaxNameLength = 21;
const int kMaxLineLength = 40;
const char FILENAME[15] = "countries.txt";
const int kCharInputLen = 3;
const int kMaxWeightLen = 7;

struct ParcelNode {
    char* destination;
    int weight;
    float value;
    struct ParcelNode* left;
    struct ParcelNode* right;
};

struct Tree {
    struct ParcelNode* root;
};

void replaceChar(char* input, char oldc, char newc);
unsigned long hash(char* str);
void insertIntoHashtable(struct Tree* hashtable[], char* destination, int weight, float value);
struct ParcelNode* createNode(char* destination, int weight, float value);
void insertIntoTree(struct Tree* root, ParcelNode* newNode);
void insertIntoTreeByWeight(ParcelNode* parent, ParcelNode* newNode);
void printTree_rec(ParcelNode* node, int prevLength, char prevDir); //TEMP
void printTree(ParcelNode* node);
void printParcel(ParcelNode* node);
void printTreeAfterW(ParcelNode* node, int weight);
void printTreeUptoW(ParcelNode* node, int weight);
int getTotalWeight(ParcelNode* tree);
float getTotalValue(ParcelNode* node);

int main(void) {
    struct Tree* hashtable[kNumBuckets] = {};

    //---- file I/O ----

    FILE* pParcelsFile = NULL;
    if ((pParcelsFile = fopen(FILENAME, "r")) == NULL) {
        printf("File open error\n");
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
        printf("File close error\n");
        return 1;
    }

    //---- menu loop ----
    
    bool loopy = true;
    while (loopy) {
        printf("1. Enter country name and display all the parcel details\n"
               "2. Enter country and weight pair\n"
               "3. Display the total parcel load and valuation for the country\n"
               "4. Enter the country name and display the cheapest and most expensive parcel's details\n"
               "5. Enter the country name and display the lightest and heaviest parcel for the country\n"
               "6. Exit the application\n");
        printf("Enter your choice: ");
        char menuInputBuffer[kCharInputLen] = "";
        fgets(menuInputBuffer, kCharInputLen, stdin);
        int menuInput = atoi(menuInputBuffer);

        char countryBuffer[kMaxNameLength] = "";
        Tree* itsTree = NULL;
        if (menuInput >= 1 && menuInput <= 5) {
            printf("Enter country name: ");
            fgets(countryBuffer, kMaxNameLength, stdin);
            replaceChar(countryBuffer, '\n', '\0');
            itsTree = hashtable[hash(countryBuffer) % kNumBuckets];
            if (!itsTree) { //empty bucket
                printf("Invalid country name.\n");
                continue;
            }
        }
        if (menuInput == 1) { // Enter country name and display all parcel details
            printTree(itsTree->root);
            printTree_rec(itsTree->root, -1, ' '); //TEMP
        }
        else if (menuInput == 2) { // Enter country and weight pair
            printf("Enter weight: ");
            char weightBuffer[kMaxWeightLen] = "";
            fgets(weightBuffer, kMaxWeightLen, stdin);
            int inputtedWeight = atoi(weightBuffer);
            char inputHiLo = '\0';
            printf("Enter '+' for greater weights, '-' for lesser weights: ");
            char hiLoBuffer[kCharInputLen] = "";
            fgets(hiLoBuffer, kCharInputLen, stdin);
            sscanf(hiLoBuffer, "%c", &inputHiLo);
            if (inputHiLo == '+') {
                printTreeAfterW(itsTree->root, inputtedWeight);
            }
            else if (inputHiLo == '-') {
                printTreeUptoW(itsTree->root, inputtedWeight);
            }
            else {
                printf("invalid input.\n");
                continue;
            }
        }
        else if (menuInput == 3) { // Display total parcel load and valuation for the country
            int totalWeight = getTotalWeight(itsTree->root);
            float totalValues = getTotalValue(itsTree->root);
            printf("Total parcel loads: %d, Total parcel values: %.2f\n", totalWeight, totalValues);
        }
        else if (menuInput == 4) { // Display cheapest and most expensive parcel's details
            //ParcelNode* cheapest = findCheapest(itsTree);
            //ParcelNode* expensivest = findExpensivest(itsTree);
            //printParcel(cheapest);
            //printParcel(expensivest);
        }
        else if (menuInput == 5) { // Display lightest and heaviest parcel for the country
            //ParcelNode * lightest = findLightest(itsTree);
            //ParcelNode* heaviest = findHeaviest(itsTree);
            //printParcel(lightest);
            //printParcel(heaviest);
        }
        else if (menuInput == 6) { // Exit the application
            loopy = false;
        }
        else { // Default case for invalid input
            printf("Invalid option selected. Please enter a number between 1 and 6.\n");
        }
    }

    //---- free memory ----

    return 0;
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

/*
* FUNCTION: insertIntoHashtable
* DESCRIPTION: insert info into hashtable. finds hash index to insert into, creates ParcelNode, 
               then calls on insertIntoTree. initializes Trees as needed.
* PARAMETERS: Tree* hashtable[]: array of Trees to insert into
              char* country: destination of parcel
*             int weight: weight of parcel in g
*             float value: value of parcel in $
* RETURNS: none
*/
void insertIntoHashtable(Tree* hashtable[], char* destination, int weight, float value) {
    int hashIndex = hash(destination) % kNumBuckets;
    struct ParcelNode* newNode = createNode(destination, weight, value);
    if (hashtable[hashIndex] == NULL) {
        hashtable[hashIndex] = (struct Tree*)malloc(sizeof(struct Tree));
        hashtable[hashIndex]->root = NULL;
    }
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

/*
* FUNCTION: insertIntoTree
* DESCRIPTION: insert ParcelNode into Tree. if not root, insert into root. 
               else call recursive function to insert by weight.
* PARAMETERS: Tree* tree: tree to insert into
*             ParcelNode* newNode: node to insert
* RETURNS: none
*/
void insertIntoTree(Tree* tree, ParcelNode* newNode) {
    if (!(tree->root)) { //root is NULL
        tree->root = newNode;
    }
    else {
        insertIntoTreeByWeight(tree->root, newNode);
    }
}

/*
* FUNCTION: insertIntoTreeByWeight
* DESCRIPTION: insert ParcelNode in order by weight, using recursion.
* PARAMETERS: ParcelNode* parent: parent node to check
*             ParcelNode* newNode: node to insert
* RETURNS: none
*/
void insertIntoTreeByWeight(ParcelNode* parent, ParcelNode* newNode) {
    if (newNode->weight < parent->weight) {
        if (parent->left) {
            insertIntoTreeByWeight(parent->left, newNode);
        }
        else {
            parent->left = newNode;
        }
    }
    else {
        if (parent->right) {
            insertIntoTreeByWeight(parent->right, newNode);
        }
        else {
            parent->right = newNode;
        }
    }
}

/*
* FUNCTION: printTree
* DESCRIPTION: recursively print tree nodes.
* PARAMETERS: ParcelNode* node: node to print and recursively call on
* RETURNS: none
*/
void printTree(ParcelNode* node) {
    if (node) {
        printTree(node->left);
        printParcel(node);
        printTree(node->right);
    }
}

/*
* FUNCTION: printParcel
* DESCRIPTION: print 1 ParcelNode's details.
* PARAMETERS: ParcelNode* node: node to print
* RETURNS: none
*/
void printParcel(ParcelNode* node) {
    printf("Destination: %s  Weight:  %5d  Value:  %7.2f\n",
        node->destination, node->weight, node->value);
}

/*
* FUNCTION: printTreeAfterW
* DESCRIPTION: recursively print tree nodes, only those with greater weight than given.
* PARAMETERS: ParcelNode* node: node to print and recursively call on
*             int weight: cutoff point for weight.
* RETURNS: none
*/
void printTreeAfterW(ParcelNode* node, int weight) {
    if (node) {
        if (node->weight < weight) {
            printTreeAfterW(node->right, weight);
        }
        else {
            printTreeAfterW(node->left, weight);
            printParcel(node);
            printTreeAfterW(node->right, weight);
        }
    }
}

/*
* FUNCTION: printTreeAfterW
* DESCRIPTION: recursively print tree nodes, only those with less weight than given.
* PARAMETERS: ParcelNode* node: node to print and recursively call on
*             int weight: cutoff point for weight.
* RETURNS: none
*/
void printTreeUptoW(ParcelNode* node, int weight) {
    if (node) {
        if (node->weight > weight) {
            printTreeAfterW(node->left, weight);
        }
        else {
            printTreeAfterW(node->left, weight);
            printParcel(node);
            printTreeAfterW(node->right, weight);
        }
    }
}

/*
* FUNCTION: getTotalWeight
* DESCRIPTION: sums up the weight of all parcels. recursively
* PARAMETERS: ParcelNode* node: node to add its weight and recursively call on.
* RETURNS: none
*/
int getTotalWeight(ParcelNode* node) {
    if (node) {
        return node->weight + getTotalWeight(node->left) + getTotalWeight(node->right);
    }
    else {
        return 0;
    }
}

/*
* FUNCTION: getTotalValue
* DESCRIPTION: sums up the value of all parcels. recursively
* PARAMETERS: ParcelNode* node: node to add its value and recursively call on.
* RETURNS: none
*/
float getTotalValue(ParcelNode* node) {
    if (node) {
        return node->value + getTotalValue(node->left) + getTotalValue(node->right);
    }
    else {
        return 0;
    }
}


//TEMP PRINTER

const int SPACING = 8;
void printTree_rec(ParcelNode* node, int prevLength, char prevDir) {
    int nextLength = (prevLength >= 0) ? (prevLength + SPACING + 1) : 0;
    char dirSym = (prevDir == 'L') ? '/' : (prevDir == 'R') ? '\\' : ' ';

    if (node) {
        
        printTree_rec(node->left, nextLength, 'L');

        //std::string nodedata = std::string(node->destination) + ", " + std::to_string(node->weight) + ", " + std::to_string(node->value);
        std::string nodedata = std::to_string(node->weight);
        if (prevLength >= 0) {
            printf("%*s%c%s%s\n", prevLength, "", dirSym, std::string(SPACING, '-').c_str(), nodedata.c_str());
        }
        else {
            printf("%s\n", nodedata.c_str());
        }

        printTree_rec(node->right, nextLength, 'R');
    }
}
