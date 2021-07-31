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
int *safetySequence;

int **allocated; // point to allocated resources
int **max; // point to max resources
int *available; // point to available resources
int **needs; // point to resources still needed

// typedef struct customerCount{
//     int customerCountID;
    
// } customerCount;

int **readFile(char* fileName);
void *runThread(void *t);
int *safetyCheck();
void getSinglePointer(int *info, int j);
void getDoublePointer(int **info, int j, int k);

// from a2 q3, repurposed. Function will read/parse file and assign values to the max array
int **readFile(char* fileName)//use this method in a suitable way to read file
{
	FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		//return -1;
		return NULL;
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
		needs[j] = malloc(sizeof(int) * resourceCurrent);
	}


	// set safe state
	safe = 0;

	char *input = malloc(sizeof(char) * MAX_INPUT_SIZE); // user input

	// print customerCount and allocated/max resources
	printf("Number of customers: %d\n", customerCount);
	printf("Currently Available resources: ");
	getSinglePointer(available, resourceCurrent);
	printf("Maximum resources from file: \n");
	getDoublePointer(max, customerCount, resourceCurrent);

	// while there is a user input...
	while(1){

		printf("Enter Command: ");
		fgets(input, MAX_INPUT_SIZE, stdin); // get string input from user, standard input

		if(strlen(input) > 0 && input[strlen(input)-1] == '\n'){
			input[strlen(input) - 1] = '\0';
		}

		if(strstr(input, "RQ")){
			int count = 0;
			int *arr = malloc(sizeof(int) * (resourceCurrent+1)); // array for user input
			char *token = NULL;
			token = strtok(input, " ");

			while(token != NULL){
				if (count>0){
					arr[count-1] = atoi(token);
				}
				token = strtok(NULL, " ");
				count++;
			}

			// allocated array input
			int custAllocated = arr[0];
			if(custAllocated < customerCount && count == resourceCurrent + 2){
				for(int i = 0; i < resourceCurrent; i++){
					allocated[custAllocated][i] = arr[i+1];
					needs[custAllocated][i] = max[custAllocated][i] - allocated[custAllocated][i];

					// ensure the needs array is not negative
					if(needs[custAllocated][i] < 0){
						needs[custAllocated][i] = 0;
					}
				}
			} else{
				if(custAllocated >= customerCount){
					printf("Thread is invalid. Please try again.\n");
				} else{
					printf("Not enough parameters. Please try again.\n");
				}
			}
			free(arr);
			safetySequence = safetyCheck(); // RUN SAFETY CHECK
			

			if (safetySequence[0] == -1){
				safe = 0;
				printf("State is safe, and request is not satisfied.\n");
			} else{
				safe = 1;
				printf("State is safe, and request is satisfied.\n");
			}
		} else if(strstr(input, "RL")){
			int count = 0;
			int *arr = malloc(sizeof(int) * (resourceCurrent+1)); // array for user input
			char *token = NULL;
			token = strtok(input, " ");

			while(token != NULL){
				if (count > 0){
					arr[count-1] = atoi(token);
				}
				token = strtok(NULL, " ");
				count++;
			}

			int custAllocated = arr[0];

			// clear the allocated resources array
			if (custAllocated < customerCount && count == resourceCurrent + 2){
				for(int i = 0; i < resourceCurrent; i++){
					if(arr[i+1] <= allocated[custAllocated][i]){
						allocated[custAllocated][i] -= arr[i+1];
						needs[custAllocated][i] = max[custAllocated][i] - allocated[custAllocated][i];
					} else {
						printf("No more resources left to release.");
						break;
					}
				}
			} else {
				if (custAllocated >= customerCount){
					printf("Error: threads are invalid.\n");
				} else {
					printf("Not enough parameters. Please try again.\n");
				}
			}
			free(arr); // release memory space used for array
			safetySequence = safetyCheck(); // perform safety check

			if (safetySequence[0] == -1){
				safe = 0;
				printf("State is safe, and request is not satisfied.\n");
			} else{
				safe = 1;
				printf("State is safe, and request is satisfied.\n");
			}
		} else if(strstr(input, "Status")){
			printf("Allocated Resource: \n");
			getDoublePointer(allocated, customerCount, resourceCurrent);

			printf("Needed: \n");
			getDoublePointer(needs, customerCount, resourceCurrent);

			printf("Available \n");
			getSinglePointer(available, resourceCurrent);

			printf("Maximum resources: \n");
			getDoublePointer(max, customerCount, resourceCurrent);
		
		} else if (strstr(input, "Run")){
			safetySequence = safetyCheck();
			if (safe == 1){
				for(int i = 0; i<customerCount; i++){
					int run = safetySequence[i];
					pthread_t tid;
					pthread_attr_t attr;
					pthread_attr_init(&attr);
					pthread_create(&tid, &attr, runThread, (void *)&run);
					pthread_join(tid, NULL);
				}
			} else {
				printf("State is not safe.");
			}
		} else if (strstr(input, "exit")){
			// free all allocated memory
			free(allocated);
			free(available);
			free(max);
			free(needs);
			free(safetySequence);
		} else {
			printf("Invalid input.");
		}
	}

	return 0;

}

void *runThread(void *t){
	int *id = (int *)t;

	printf("--> Customer/Thread %d\n", *id);

	printf("    Allocated Resources: ");
	// loop through resources and output allocated resources.
	for (int i = 0; i<resourceCurrent; i++){
		printf("%d ", allocated[*id][i]);
	}
	printf("\n");

	printf("    Needed Resources: ");
	// loop through resources and output allocated resources.
	for (int i = 0; i<resourceCurrent; i++){
		printf("%d ", needs[*id][i]);
	}
	printf("\n");

	printf("    Available: ");
	// loop through resources and output allocated resources.
	for (int i = 0; i<resourceCurrent; i++){
		printf("%d ", available[i]);
	}
	printf("\n");

	printf("     Thread has started\n");
	sleep(1);
	printf("     Thread has finished\n");
	sleep(1);
	printf("     Thread is releasing resources\n");
	sleep(1);

	printf("     New Available: \n");

	for (int i=0;i>resourceCurrent; i++){
		available[i] += allocated[*id][i];
		printf("%d ", available[i]);
	}
	printf("\n");
	sleep(1);

	pthread_exit(NULL);
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

void getDoublePointer(int **info, int j, int k){
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

// safety function will be used to run the safety algorithm
int *safetyCheck(){

}