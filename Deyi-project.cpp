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
void showNode(Parcel* root);
void showWholeTree(Parcel* root);
void showHigher(Parcel* root, int newWeight);
void showLower(Parcel* root, int newWeight);
int CaculatetotalLoad(Parcel* root);
float  CaculatetotalValutation(Parcel* root);

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
			sscanf_s(buffer, "%[^,], %d, %f", destination, BUFFER_LENGTH, &weight, &value);
			deleteRC(destination);
			

			insertHashTable(countries, destination, weight, value);
		}
	}
	if (fclose(ptr) != 0)
	{
		printf("Error of file closing\n");
		exit(EXIT_FAILURE);
	}

	// User Interface
	char userInput = 0;
	char userInputString[BUFFER_LENGTH] = {};
//	while (userInput != 6)
//	{				
//		printf("1. Enter country name and display all the parcels details.\n");
//		printf("2. Enter country and weight pair.\n");
//		printf("3. Display the total parcel load and valuation for the country.\n");
//		printf("4. Enter the country name and display cheapest and most expensive parcel's details.\n");
//		printf("5. Enter the country name and display lightest and heaviest parcel for the country.\n");
//		printf("6. Exit the application.\n");
//		printf("Please choose from the menu:");
//		userInput = getchar();
//		printf("CHar is : %c \n", userInput);
//
//		/*switch (userInput)
//		{
//		case 1:
//			printf("Please enter country name: ");
//			fgets(userInputString, BUFFER_LENGTH, stdin);
//			printf("U input: %s", userInputString);
//			showWholeTree(countries->hashTable[hashfunction(userInputString)]);
//			break;
//		case 2:
//			printf("");
//			break;
//		case 3:
//			printf("");
//			printf("Total load is %d.\n", totalLoad);
//			printf("Total valuation is %f.\n", valuation);
//			break;
//		case 4:
//			printf("");
//			break;
//		case 5:
//			printf("");
//			break;
//		case 6:
//			printf("");
//			break;
//		default:
//			break;
//		}*/
//		
//
//	////}
//
//
	//test:
	//showWholeTree(countries->hashTable[0]);
	// 
	//showLower(countries->hashTable[0], 5132);

	/*int load = CaculatetotalLoad(countries->hashTable[0]);
	float totalvalue = CaculatetotalValutation(countries->hashTable[0]);
	printf("%d, %f", load, totalvalue);*/



	//freeLinkedList(); 
	return 0;
}

// the cumulative total valuation of all the parcels.
float  CaculatetotalValutation(Parcel* root)
{
	float valuation = 0.0;
	if (root == NULL)
	{
		return 0;
	}
	valuation += root->value;
	valuation += CaculatetotalLoad(root->left);
	valuation += CaculatetotalLoad(root->right);
	return valuation;
}

// the cumulative parcel load of all the parcels.
int CaculatetotalLoad(Parcel* root)
{
	int totalLoad = 0;
	
	if (root == NULL)
	{
		return 0;
	}
	totalLoad += root->weight;
	totalLoad += CaculatetotalLoad(root->left);
	totalLoad += CaculatetotalLoad(root->right);
	return totalLoad;
}


// display all the parcel for given country whose weight is lower than weight entered.
void showLower(Parcel* root, int newWeight)
{
	if (root == NULL)
	{
		return;
	}

	if (root->weight < newWeight)
	{
		showNode(root);
		showWholeTree(root->left);
		showLower(root->right, newWeight);
	}
	else if (root->weight > newWeight)
	{
		showLower(root->left, newWeight);
	}
	else
	{
		showWholeTree(root->left);
	}
}

// display all the parcel for given country whose weight is higher than weight entered.
void showHigher(Parcel* root, int newWeight)
{
	if (root == NULL)
	{
		return;
	}

	if (root->weight < newWeight)
	{		
		showHigher(root->right, newWeight);
	}
	else if (root->weight > newWeight)
	{
		showNode(root);
		showWholeTree(root->right);
		showLower(root->left, newWeight);
	}
	else
	{
		showWholeTree(root->right);
	}
}

// mid-left-right
void showNode(Parcel * root)
{
	printf("Destination: %s\n", root->destination);
	printf("Weight: %d\n", root->weight);
	printf("Value: %f\n\n", root->value);
}

// mid-left-right
void showWholeTree(Parcel* root)
{
	if (root == NULL)
	{
		return;
	}
	showNode(root);

	showWholeTree(root->left);
	showWholeTree(root->right);
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
		// important: assiged to the child
		root->left = insertTree(root->left, newNode);
		// important: don't return here, if you want the right tree to grow correctly
	}
	// insert right subtree
	if (root->weight < newNode->weight)
	{
		root->right =