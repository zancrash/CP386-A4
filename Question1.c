/*
------------------------------
Project: Assignment 4
File: Question1.c
------------------------------
Authors: Muhammad Ali
		 Nicolas Lozano

IDs:	191651560
		150701040

Github: https://github.com/zancrash/CP386-A4
------------------------------
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

#define FILE_IN "sample4_in.txt"
#define MAX_INPUT_SIZE 256 

int customerCount;
int resourceCurrent;
int safe;
int *safetySequence;

int **allocated; // point to allocated resources
int **max; // point to max resources
int *available; // point to available resources
int **needs; // point to resources still needed

// Function declarations
int **readFile(char* filename);
void *runThread(void *t);
int *safetyCheck();
void getSinglePointer(int *info, int j);
void getDoublePointer(int **info, int j, int k);
void run(int safe, int customerCount);

// from a2 q3, repurposed. Function will read/parse file and assign values to the max array
int **readFile(char *filename) { //use this method in a suitable way to read file
	FILE *in = fopen(filename, "r");
	if (!in) {
		printf("Child A: Error in opening input file... exiting with error code -1\n");
		return NULL;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';
	while (!feof(in)) {
		char line[100];
		if (fgets(line, 100, in) != NULL) {
			strncat(fileContent, line, strlen(line));
		}
	}
	fclose(in);

	char *userCommand = NULL;
	//int threadCount = 0;
	char *fileCopy = (char *)malloc((strlen(fileContent) + 1) * sizeof(char));
	strcpy(fileCopy, fileContent);
	userCommand = strtok(fileCopy, "\r\n");
	while (userCommand != NULL) {
		customerCount++;
		userCommand = strtok(NULL, "\r\n");
	}
	strcpy(fileCopy, fileContent);
	char *lines[customerCount];
	int i = 0;
	userCommand = strtok(fileCopy, "\r\n");
	while (userCommand != NULL) {
		lines[i] = malloc(sizeof(userCommand) * sizeof(char));
		strcpy(lines[i], userCommand);
		i++;
		userCommand = strtok(NULL, "\r\n");
	}
	int **max = malloc(sizeof(int *) * customerCount);
	for (int j = 0; j < customerCount; j++) {
		int *temp = malloc(sizeof(int) * resourceCurrent);
		char *token = NULL;
		int k = 0;
		token = strtok(lines[j], ",");
		while (token != NULL) {
			temp[k] = atoi(token);
			k++;
			token = strtok(NULL, ",");
		}
		max[j] = temp;
	}
	return max;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Needs more parameters...\n");
		return -1;
	}
	resourceCurrent = argc - 1;

	// available array initialization
	available = malloc(sizeof(int) + resourceCurrent);
	for (int i = 1; i < argc; i++) {
		available[i - 1] = atoi(argv[i]);
	}

	// Memory allocation for arrays
	max = readFile(FILE_IN);
	allocated = malloc(sizeof(int *) * customerCount);
	needs = malloc(sizeof(int *) * customerCount);
	for (int i = 0; i < customerCount; i++) {
		allocated[i] = malloc(sizeof(int) * resourceCurrent);
		needs[i] = malloc(sizeof(int) * resourceCurrent);
	}

	safe = 0;
	char *inputString = malloc(sizeof(char) * MAX_INPUT_SIZE);

	printf("Number of customers: %d\n", customerCount);
	printf("Currently Available resources: ");
	getSinglePointer(available, resourceCurrent);
	printf("Maximum Resources from file:\n");
	getDoublePointer(max, customerCount, resourceCurrent);

	// While there is a user input...
	while (1) {
		printf("Enter command: ");
		fgets(inputString, MAX_INPUT_SIZE, stdin);

		if (strlen(inputString) > 0 && inputString[strlen(inputString) - 1] == '\n') {
			inputString[strlen(inputString) - 1] = '\0';
		}

		if (strstr(inputString, "RQ")) {

			int c = 0;
			int *arr = malloc(sizeof(int) * (resourceCurrent+1)); // array for user input
			char *token = NULL;
			token = strtok(inputString, " ");

			while(token != NULL){
				if (c>0){
					arr[c-1] = atoi(token);
				}
				token = strtok(NULL, " ");
				c++;
			}

			// allocated array input
			int custAllocated = arr[0];
			if(custAllocated < customerCount && c == resourceCurrent + 2){
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
				printf("State is safe, and request is satisfied.\n");
			} else{
				safe = 1;
				printf("State is not safe.\n");
			}

		} else if (strstr(inputString, "RL")){
						int c = 0;
			int *arr = malloc(sizeof(int) * (resourceCurrent+1)); // array for user input
			char *token = NULL;
			token = strtok(inputString, " ");

			while(token != NULL){
				if (c > 0){
					arr[c-1] = atoi(token);
				}
				token = strtok(NULL, " ");
				c++;
			}

			int custAllocated = arr[0];

			// clear the allocated resources array
			if (custAllocated < customerCount && c == resourceCurrent + 2){
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
		}


		else if (strstr(inputString, "Status")) {
			printf("Allocated Resource: \n");
			getDoublePointer(allocated, customerCount, resourceCurrent);

			printf("Needed: \n");
			getDoublePointer(needs, customerCount, resourceCurrent);

			printf("Available \n");
			getSinglePointer(available, resourceCurrent);

			printf("Maximum resources: \n");
			getDoublePointer(max, customerCount, resourceCurrent);
		}
		else if (strstr(inputString, "Run")) {
			run(safe, customerCount);
		}
		else if (strstr(inputString, "exit")) {
			free(available);
			free(max);
			free(allocated);
			free(needs);
			free(safetySequence);
		}
		else {
			printf("\"%s\" is an invalid input. List of commands: \"RQ\",\"RL\",\"Status\",\"Run\",\"exit\".\n", inputString);
		}
	}
	return 0;
}

void *runThread(void *t) {
	int *id = (int *)t;

	printf("--> Customer/Thread %d\n", *id);
	printf("    Allocated Resources: ");

	// loop through resources and output allocated resources.
	for (int i = 0; i<resourceCurrent; i++) {
		printf("%d ", allocated[*id][i]);
	}

	printf("\n");
	printf("    Needed Resources: ");

	// loop through resources and output allocated resources.
	for (int i = 0; i<resourceCurrent; i++) {
		printf("%d ", needs[*id][i]);
	}
	
	printf("\n");
	printf("    Available: ");
	
	// loop through resources and output allocated resources.
	for (int i = 0; i<resourceCurrent; i++) {
		printf("%d ", available[i]);
	}
	printf("\n");

	printf("> Thread has started\n");
	sleep(1);
	printf("> Thread has finished\n");
	sleep(1);
	printf("> Thread is releasing resources\n");
	sleep(1);

	printf("> New Available: \n");

	for (int i=0;i>resourceCurrent; i++) {
		available[i] += allocated[*id][i];
		printf("%d ", available[i]);
	}
	printf("\n");
	sleep(1);

	pthread_exit(NULL);
}



//
void getSinglePointer(int *info, int j) {
	for (int i = 0; i < j; i++) {
		printf("%d", info[i]);
		if(i < j-1) {
			printf(" ");
		}
	}
	printf("\n");
}

//
void getDoublePointer(int **info, int j, int k) {
	for (int i = 0; i < j; i++) {
		for (int y = 0; y < k; y++) {
			printf("%d", info[i][y]);
			if(y < k-1) {
				printf(" ");
			}
		}
		printf("\n");	
	}
}

// Request resources
void request(int resourceCurrent, int customerCount, int **need, int **allocated, int **max, int safe, char *inputString) {
	int *arr = malloc(sizeof(int) * (resourceCurrent + 1)); // input array
	
	char *token = NULL;
	token = strtok(inputString, " ");

	int i = 0;
	while (token != NULL) {
		if (i > 0) {
			arr[i - 1] = atoi(token);
		}
		token = strtok(NULL, " ");
		i++;
	}

	// customer to allocated insert into allocaed array
	int customerAllocated = arr[0];
	if (customerAllocated < customerCount && i == resourceCurrent + 2) {
		for (int j = 0; j < resourceCurrent; j++) {
			allocated[customerAllocated][j] = arr[j + 1];
			need[customerAllocated][j] = max[customerAllocated][j] - allocated[customerAllocated][j];
			// need has to be positive
			if (need[customerAllocated][j] < 0)
			{
				need[customerAllocated][j] = 0;
			}
		}
	}
	else {
		if (customerAllocated >= customerCount) {
			printf("Thread is invalid. Please try again.\n");
		}
		else {
			printf("Not enough parameters. Please try again.\n");
		}
	}
	free(arr);
	safetySequence = safetyCheck();
	if (safetySequence[0] == -1){
				safe = 0;
				printf("State is safe, and request is not satisfied.\n");
			} else{
				safe = 1;
				printf("State is safe, and request is satisfied.\n");
			}
	return;
}

void run(int safe, int customerCount) {
	safetySequence = safetyCheck();
	if (safe == 1) {
		for (int i = 0; i < customerCount; i++) {
			int runThread = safetySequence[i];
			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tid, &attr, runThread, (void *)&runThread);
			pthread_join(tid, NULL);
		}
	}
	else {
		printf("State is not safe. \n");
	}
}

// safety function will be used to run the safety algorithm
int *safetyCheck() {

	int *finished = malloc(sizeof(int) * customerCount);
	int *safetySequence = malloc(sizeof(int) * customerCount);
	int *temp = malloc(sizeof(int) * resourceCurrent);

	for (int i = 0; i < resourceCurrent; i++) { // what is avialble to work
		temp[i] = available[i];
	}

	int j = 0;
	while (j < customerCount) {
		int safe = 0;
		for (int i = 0; i < customerCount; i++) {
			if (finished[i] == 0) {
				int safety = 1;
				for (int j = 0; j < resourceCurrent; j++) {
					if (temp[j] <= needs[i][j]) {
						safety = 0;
						break;
					}
				}
				if (safety == 1) {
					safetySequence[j] = i;
					finished[i] = 1;
					safe = 1;
					j++;

					for (int j = 0; j < resourceCurrent; j++) {
						temp[j] += allocated[i][j];
					}
				}
			}
		}
		if (safe == 0) {
			for (int m = 0; m < customerCount; m++) {
				safetySequence[m] = -1;
			}
			free(finished);
			free(temp);

			return safetySequence;

			//// break();
		}
	}
	free(finished);
	free(temp);

	return safetySequence;
}