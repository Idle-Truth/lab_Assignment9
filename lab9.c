#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
    struct RecordType *pRecord;
	struct HashType *next;
    int key;
};

// Compute the hash function
int hash(int x)
{
    return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;

}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].pRecord != NULL)
        {
            printf("Index %d -> ", i);
            for (struct HashType *current = &pHashArray[i]; current != NULL; current = current->next)
            {
                if (current->pRecord != NULL) // Check if the current node has a record
                {
                    printf("%d %c %d  ", current->pRecord->id, current->pRecord->name, current->pRecord->order);
                    printf("->");
                }

            }
            printf("\n");
        }
    }
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
    struct HashType *pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * 23);
    if (!pHashArray) {
        perror("Failed to allocate memory for hash table");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 23; ++i)
    {
        pHashArray[i].pRecord = NULL;
        pHashArray[i].next = NULL; // Ensure the 'next' pointer is also initialized to NULL
    }
    for (int i = 0; i < recordSz; ++i)
    {
        int key = hash(pRecords[i].id);
        struct HashType *newNode = (struct HashType*)malloc(sizeof(struct HashType));
        newNode->pRecord = &pRecords[i];
        newNode->next = NULL;
        newNode->key = key;
        if (pHashArray[key].pRecord == NULL) // No collision
        {
            pHashArray[key].pRecord = &pRecords[i];
            pHashArray[key].next = NULL; // No next node
        }
        else // Collision, add to the front of the list
        {
            newNode->next = pHashArray[key].next; // Point new node's next to the current first node
            pHashArray[key].next = newNode; // Update head's next to new node
        }
    }
    displayRecordsInHash(pHashArray, 23);
    free(pHashArray);
    free(pRecords);
    return 0;
}
