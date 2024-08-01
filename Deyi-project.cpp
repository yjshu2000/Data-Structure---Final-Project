#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS_NUMBER 127
#define BUFFER_LENGTH 5000

typedef struct Parcel 
{
	char* destination;
	int weight;
	float value;
	Parcel* left;
	Parcel* right;
};

typedef struct HashTable
{
	Parcel* hashTable[BUCKETS_NUMBER];
};


HashTable* initializeHashTable(void);
Parcel* creatNode(char* destination, int weight, float value);
int hashfunction(char* str);
void deleteRC(char* record);
void insertHashTable(HashTable* countries, char* destination, int weight, float value);
Parcel* insertTree(Parcel* root, Parcel* newNode);


int main(void)
{
	HashTable* countries = initializeHashTable();
	char buffer[BUFFER_LENGTH] = {};
	FILE* ptr = NULL;
	char destination[BUFFER_LENGTH] = {};
	int weight = 0;
	float value = 0.0;

	// file IO
	if (fopen_s(&ptr, "./countries.txt", "r") != 0)
	{
		printf("Error of file Openning.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		while (!feof(ptr)) 
		{
			if (fgets(buffer, BUFFER_LENGTH, ptr) == NULL)
			{
				printf("Error of file Reading\n");
				exit(EXIT_FAILURE);
			}
			// extract datas:
			deleteRC();
			strcpy_s(destination, BUFFER_LENGTH, );
			weight = ;
			value = ;
			insertHashTable(countries, destination, weight, value);
	}
	
	if (fclose(ptr) != 0)
	{
		printf("Error of file closing\n");
		exit(EXIT_FAILURE);
	}

	// User Interface

	while (true)
	{
		printf(
			"1. Enter country name and display all the parcels details.\n"
			"2. Enter country and weight pair.\n"
			"3. Display the total parcel load and valuation for the country.\n"
			"4. Enter the country name and display cheapest and most expensive parcel’s details.\n"
			"5. Enter the country name and display lightest and heaviest parcel for the country.\n"
			"6. Exit the application.\n");
	}

	return 0;
}


void showTree(Parcel * root)
{

}
Parcel* insertTree(Parcel* root, Parcel* newNode)
{
	// insert root
	if (root == NULL)
	{
		root = newNode;
		return root;
	}

	// insert left subtree
	if (root->weight > newNode->weight )
	{
		insertTree(root->left, newNode);
		return root;
	}
	// insert right subtree
	if (root->weight < newNode->weight)
	{
		insertTree(root->right, newNode);
		return root;
	}
}

void insertHashTable(HashTable* countries, char* destination, int weight, float value)
{
	Parcel* newNode = creatNode(destination, weight, value);
	int hashNumber = hashfunction(destination);
	Parcel* root = countries->hashTable[hashNumber];
	
	insertTree(root, newNode);

}

Parcel* creatNode(char* destination, int weight, float value)
{
	Parcel* newNode = (Parcel*)malloc(sizeof(Parcel));
	if (newNode == NULL)
	{
		printf("Error of memory.\n");
		exit(EXIT_FAILURE);
	}

	newNode->destination = (char*)malloc(strlen(destination) + 1);
	if (newNode->destination == NULL)
	{
		printf("Error of memory.\n");
		exit(EXIT_FAILURE);
	}

	strcpy_s(newNode->destination, strlen(destination) + 1, destination);
	newNode->value = value;
	newNode->weight = weight;
	newNode->left = NULL;
	newNode->right = NULL;
}

HashTable* initializeHashTable(void)
{
	HashTable* Hashtable = (HashTable * ) malloc(sizeof(HashTable));

	if (Hashtable == NULL)
	{
		printf("Error of memory.\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < BUCKETS_NUMBER; i++)
	{
		Hashtable->hashTable[i] = NULL;
	}
}

/* ================================================================ */
/* FUNCTION : deleteRC( record)                                     */
/* PURPOSE  : This function is to delete the RC in the end of string*/
/* INPUTS   : record - the string                                   */
/* RETURNS  : None                                                  */
/* ================================================================ */
void deleteRC(char* record)
{
	char* whereRC = NULL;
	whereRC = strchr(record, '\n');
	if (whereRC)
	{
		*whereRC = '\0';
	}
}


/* ================================================================ */
/* FUNCTION : hashfunction( str)                                    */
/* PURPOSE  : use algorim to get a int between 0 and 127 from input */
/* INPUTS   : str - string to hash                                  */
/* RETURNS  : a int to use as index in a hashtable                  */
/* Author   : Dan Bernstein                                         */
/*            I got it from lecture                                 */
/* ================================================================ */
int hashfunction(char* str)
{
	unsigned long hash = 5381;
	int c;
	while (c = *str++)
	{
		hash = ((hash << 5) + hash) + c; // hash * 33 + c
	}

	return hash % BUCKETS_NUMBER;
}

/* ================================================================ */
/* FUNCTION : freeLinkedList( head)			                        */
/* PURPOSE  : free the memory that has been allocated				*/
/* INPUTS   : head - a pointer to the head of the linked list	    */
/* RETURNS  : none									                */
/* ================================================================ */
void freeLinkedList(struct Parcel** head)
{
	struct Parcel* current = *head;
	while (current != NULL)
	{

	}
}