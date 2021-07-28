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

// int allocated[4] = {}; // array to store allocated resources.
// int max[4] = {}; // array to store maximum required resources.
// int available[4] = {}; // array to store available resources.
// int needs[4] = {}; // array to store how many more resources are needed.

int customerCount;
int resourceCurrent;
int safe;

int **allocated; // point to allocated resources
int **max; // point to max resources
int *available; // point to available resources
int **needs; // point to resources still needed

// typedef struct customerCount{
//     int customerCountID;
    
// } customerCount;

int **readFile(char* fileName);

// from a2 q3, repurposed. Function will read/parse file and assign values to the max array
int **readFile(char* fileName)//use this method in a suitable way to read file
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
		customerCount++;
		command = strtok(NULL,"\r\n");
	}
	//*threads = (Thread*) malloc(sizeof(Thread)*threadCount);
    strcpy(fileCopy, fileContent);
	char* lines[customerCount];
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

    int **max = malloc(sizeof(int *) * customerCount);

    for (int k = 0; k < customerCount; k++){

        int *temp = malloc(sizeof(int) * resourceCurrent);
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

	if (argc<2){
		printf("Not enough parameters. Please try again.\n");
		return -1;
	}

	resourceCurrent = argc - 1;

	// initialize allocated resources array
	allocated = malloc(sizeof(int *) * customerCount);

	// populate maximum resources array
	max = readFile(FILE_IN);

	// populate available resources arr
	available = malloc(sizeof(int) + resourceCurrent);

	for (int i = 0; i < argc; i++){
		available[i-1] = atoi(argv[i]);
	}

	// initialize needed resources
	needs = malloc(sizeof(int *) * customerCount);
	for (int j = 0; j < customerCount; j++){
		allocated[j] = malloc(sizeof(int) * resourceCurrent);
		need[j] = malloc(sizeof(int) * resourceCurrent);
	}


	// set safe state
	safe = 0;

	char *input = malloc(sizeof(char) * MAX_INPUT_SIZE);

	// print customerCount and allocated/max resources
	printf("Number of customers: %d\n", customerCount);
	printf("Currently Available resources: ");
	getSinglePointer(available, resourceCurrent);
	printf("Maximum resources from file: ");
	getDoublePointer(max, customerCount, resourceCurrent);

}

// safety function will be used to run the safety algorithm
int safetyCheck(){

}

void getSinglePointer(int *info, int j){
	for (int i = 0; i < j; i++){
		printf("%d", info[i]);
		if(i < j-1){
			printf(" ");
		}
	}
	printf("\n");
}

void getDoublePointer(int *info, int j, int k){
	for (int i = 0; i < j; i++){
		for (int y = 0; y < k; y++){
			printf("%d", info[i][y]);
			if(y < k-1){
				printf(" ");
			}
		}
		printf("\n");	
	}
}