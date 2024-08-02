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
float findCheapest(Parcel* root, float cheapest);
float findExpensive(Parcel* root, float expensive);
void findLightest(Parcel* root);

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

	//printf("%f\n", findCheapest(countries->hashTable[0], 665.14));

	//findLightest(countries->hashTable[0]);

	//freeLinkedList(); 
	return 0;
}

//display lightest and heaviest parcel for the country
void findLightest(Parcel* root)
{
	Parcel* lightest = NULL;
	Parcel* heaviest = NULL;
	if (root == NULL)
	{
		printf("No parcel for this destation.\n");
		return;
	}
	Parcel* current = root;
	while (current->left != NULL)
	{
		current = current->left;
	}
	lightest = current;

	current = root;
	while (current->right != NULL)
	{
		current = current->right;
	}
	heaviest = current;

	printf("The lightest parcel is :\n");
	showNode(lightest);
	printf("The heaviest parcel is :\n");
	showNode(heaviest);
}

// display cheapest parcel’s details
float findCheapest(Parcel* root, float cheapest)
{
	if (root == NULL)
	{
		return cheapest;
	}
	if (root->value >= cheapest)
	{
		cheapest = findCheapest(root->left, cheapest);
		cheapest = findCheapest(root->right, cheapest);
	}
	else
	{
		cheapest = root->value;
		cheapest = findCheapest(root->left, cheapest);
		cheapest = findCheapest(root->right, cheapest);
	}
	return  cheapest;
}

// display most expensive parcel’s details
float findExpensive(Parcel* root, float expensive)
{
	if (root == NULL)
	{
		return expensive;
	}
	if (root->value <= expensive)
	{
		expensive = findCheapest(root->left, expensive);
		expensive = findCheapest(root->right, expensive);
	}
	else
	{
		expensive = root->value;
		expensive = findCheapest(root->left, expensive);
		expensive = findCheapest(root->right, expensive);
	}
	return  expensive;
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
	totalLoad += Caculatetota