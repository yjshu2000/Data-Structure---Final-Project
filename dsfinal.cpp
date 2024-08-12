/* heaedr cment
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <string> //delete later
#pragma warning(disable: 4996)

const int kNumBuckets = 127;
const int kMaxNameLength = 21;
const int kMaxLineLength = 40;
const char FILENAME[15] = "countries.txt";
const int menuInputLen = 3;

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
void insertIntoTree_rec(ParcelNode* parent, ParcelNode* newNode);
//void printTree_rec(ParcelNode* node, int prevLength, char prevDir); //temp
void displayAllParcels(struct Tree* hashtable[], char* destination);
void printTree_rec(ParcelNode* node);
void printParcel(ParcelNode* node);

int main(void) {
    struct Tree* hashtable[kNumBuckets] = {};

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
    
    bool loopy = true;
    while (loopy) {
        printf("1. Enter country name and display all the parcel details\n"
               "2. Enter country and weight pair\n"
               "3. Display the total parcel load and valuation for the country\n"
               "4. Enter the country name and display the cheapest and most expensive parcel's details\n"
               "5. Enter the country name and display the lightest and heaviest parcel for the country\n"
               "6. Exit the application\n");
        printf("Enter your choice: ");
        char menuInputBuffer[menuInputLen] = "";
        fgets(menuInputBuffer, menuInputLen, stdin);
        int menuInput = atoi(menuInputBuffer);

        char countryBuffer[kMaxNameLength] = "";
        Tree* itsTree = NULL;
        if (menuInput >= 1 && menuInput <= 5) {
            printf("Enter country name: ");
            fgets(countryBuffer, kMaxNameLength, stdin);
            replaceChar(countryBuffer, '\n', '\0');
            itsTree = hashtable[hash(countryBuffer) % kNumBuckets];
        }
        switch (menuInput) {
        case 1: // Enter country name and display all parcel details
            displayAllParcels(hashtable, countryBuffer);
            break;

        case 2: // Enter country and weight pair
            //also get weight??
            //??? what do we do 
            break;

        case 3: // Display total parcel load and valuation for the country

            int totalWeight = getTotalWeight(itsTree);
            float totalValues = getTotalValues(itsTree);
            printf("Total parcel loads: %d, Total parcel values: %.f", totalWeight, totalValues);
            break;

        case 4: // Display cheapest and most expensive parcel's details
            ParcelNode* cheapest = findCheapest(itsTree);
            ParcelNode* expensivest = findExpensivest(itsTree);
            printParcel(cheapest);
            printParcel(expensivest);
            break;

        case 5: // Display lightest and heaviest parcel for the country
            ParcelNode * lightest = findLightest(itsTree);
            ParcelNode* heaviest = findHeaviest(itsTree);
            printParcel(lightest);
            printParcel(heaviest);
            break;

        case 6: // Exit the application
            loopy = false;
            break;

        default:
            printf("Invalid option selected. Please enter a number between 1 and 6.\n");
            break;
        }
    }

    //free stuff; aaaa

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
               else call recursive function.
* PARAMETERS: Tree* tree: tree to insert into
*             ParcelNode* newNode: node to insert
* RETURNS: none
*/
void insertIntoTree(Tree* tree, ParcelNode* newNode) {
    if (!(tree->root)) { //root is NULL
        tree->root = newNode;
    }
    else {
        insertIntoTree_rec(tree->root, newNode);
    }
}

/*
* FUNCTION: insertIntoTree_rec
* DESCRIPTION: insert ParcelNode in order by weight, using recursion.
* PARAMETERS: ParcelNode* parent: parent node to check
*             ParcelNode* newNode: node to insert
* RETURNS: none
*/
void insertIntoTree_rec(ParcelNode* parent, ParcelNode* newNode) {
    if (newNode->weight < parent->weight) {
        if (parent->left) {
            insertIntoTree_rec(parent->left, newNode);
        }
        else {
            parent->left = newNode;
        }
    }
    else {
        if (parent->right) {
            insertIntoTree_rec(parent->right, newNode);
        }
        else {
            parent->right = newNode;
        }
    }
}

/*
* FUNCTION: displayAllParcels
* DESCRIPTION: finds the bucket index of the destination,
               and then call on printTree_rec to display the tree there.
* PARAMETERS: Tree* hashtable[]: hashtable array to find name in
*             char* destination: name to look for
* RETURNS: none
*/
void displayAllParcels(Tree* hashtable[], char* destination) {
    int hashIndex = hash(destination) % kNumBuckets;
    printTree_rec(hashtable[hashIndex]->root);
}

/*
* FUNCTION: printTree_rec
* DESCRIPTION: recursively print tree nodes.
* PARAMETERS: ParcelNode* node: node to print and recursively call on
* RETURNS: none
*/
void printTree_rec(ParcelNode* node) {
    if (node) {
        printTree_rec(node->left);
        printParcel(node);
        printTree_rec(node->right);
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


//TEMP PRINTER
/*
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
*/