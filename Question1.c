/*
 -------------------------------------
 File:    a4.c
 Project: A04
 file description
 -------------------------------------
 Authors:  Muhammad Ali
 ID:      191651560
 Email:   alix1560@mylaurier.ca
 Version  2021-07-17
 -------------------------------------
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>
#include <stdbool.h>

#define FILE_IN "sample4_in.txt"
#define MAX_INPUT_SIZE 256

typedef struct customer{ // Representation of a customer
    int customerID;
	pthread_t 
	
} Customer;

// int allocated[4] = {}; // array to store allocated resources.
// int max[4] = {}; // array to store maximum required resources.
// int available[4] = {}; // array to store available resources.
// int needs[4] = {}; // array to store how many more resources are needed.

int customer;
int resource_type;
int safe;

int **allocated; // point to allocated resources
int **max; // point to max resources
int *available; // point to available resources
int **needs; // point to resources still needed

int resourcesCount; // Number of resources given
int customerCount; //Count of customers
Customer* customers = NULL; //Stores array of clients as given in input text

// Function declarations
int **readFile(char* fileName);
int *safety(int resourceCount);
int countCustomers(char* file);


// from a2 q3, repurposed
int **readFile(char* fileName) //use this method in a suitable way to read file
{
	FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char* fileContent = (char*)malloc(((int)st.st_size+1)* sizeof(char));
	fileContent[0]='\0';	
	while(!feof(in))
	{
		char line[100];
		if(fgets(line,100,in)!=NULL)
		{
			strncat(fileContent,line,strlen(line));
		}
	}
	fclose(in);

	char* command = NULL;
	//int threadCount = 0;
	char* fileCopy = (char*)malloc((strlen(fileContent)+1)*sizeof(char));
	strcpy(fileCopy,fileContent);
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		customer++;
		command = strtok(NULL,"\r\n");
	}
	//*threads = (Thread*) malloc(sizeof(Thread)*threadCount);
    strcpy(fileCopy, fileContent);
	char* lines[customer];
	//command = NULL;
	int i=0;
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		lines[i] = malloc(sizeof(command)*sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL,"\r\n");
	}

    int **max = malloc(sizeof(int *) * customer);

    for (int k = 0; k < customer; k++){

        int *temp = malloc(sizeof(int) * resource);
        char *token = NULL;

        int j = 0;
        
        token = strtok(lines[k], ",");
        while (token!=NULL){
            temp[j] = atoi(token);
            j++;
            token = strtok(NULL, ",");
        }

        max[k] = temp;

    }

    return max;

    

// 	for(int k=0; k<threadCount; k++)
// 	{
// 		char* token = NULL;
// 		//int j = 0;
// 		token =  strtok(lines[k],";");
// 		while(token!=NULL)
// 		{
// 			strncpy((*threads + k) -> tid, token, 3); //copy thread id

// 			token = strtok(NULL, ";");

// 			if (token){
// 				(*threads + k) -> start_time = strtol(token, NULL, 10);
// 			}

// 			if (token){
// 				(*threads + k) -> lifetime = strtol(token, NULL, 10);
// 			}

// 			(*threads + k) -> executed = 0;

// //this loop tokenizes each line of input file
// //write your code here to populate instances of Thread to build a collection
// 		}
// 	}
// 	return threadCount;
}


int main(int argc, char *argv[]){
	if(argc < 2)
	{
		printf("INPUT ERROR:'Available' resources Data Missing... Error Code -1\n");
		return -1;
	}

	int resourcesCount = argc - 1;
	
}

/**
 * ================================================================
 * getCustCount - Count number of "\n" occurances in the given file
 * ================================================================
 **/
int getCustCount(char *filename) {
	
	// Variable Declarations
	FILE *fileptr;
    int count_lines = 0;
    char c;
 
    fileptr = fopen(filename, "r"); // Open File

    c = getc(fileptr); // Extract Character from File and store in variable 'c'
    
	while (c != EOF)
    {
		// Increment Count Lines when "\n" is present
        if (c == '\n')
            count_lines++;

        c = getc(fileptr);
    }

    fclose(fileptr); // Close file

    return count_lines;
}



// safety function will be used to run the safety algorithm
int *safety(int resourceCount){
	int safe = 0, isLocated = 1, safeResourceNum = 0, columnIndex = 0;

	// Array initiations
	safeResources = (int*) malloc(customerCount * sizeof(int));
	pending = (int*) malloc(resourceCount * sizeof(int));
	completed = (int*) malloc(customerCount * sizeof(int));

	for (int i = 0; i < customerCount; i++) {
		safeResources[i] = -1;
		completed[i] = 0;
	}

	for (int i = 0; i < resourceCount; i++) {
		pending[i] = Available[i];
	}

	while(isLocated == 1 && safe == 0){
		isLocated = 0;
		for (int i = 0; i < customerCount; i++){
			if (completed[i] == 0){
				columnIndex = 0;
				int j = 0;
				while (j < resourceCount){
					if (needs[i][j] == -1 || pending[j] == -1){
						safe = -1;
						break;
					}

					if (needs[i][j] > pending[j]) {
						break;
					} else {
						columnCount++;
					}
					if(columnIndex == resourceCount){
						safeResources[safeResourceNum] = i;
						safeResourceNum += 1;
						completed[i] = 1;
						isLocated = 1;

						for (int k = 0; k < resourceCount; k++){
							pending[k] += allocated[i][k];
						}
					}
					j++;
				}
			}
		}
	}


	for (int i = 0; i < customerCount; i++) {
		if (completed[i] == 0) {
			safe = -1;
		}
	}

	return safe;
}