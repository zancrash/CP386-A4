# CP386-A4

## Motivation
To demostrate knowledge of banker's algorithm by implementing it in a C program.

## Installation
To run the banker's algorithm program, the repository must be cloned and its destination must be accessed from cmd to run.
Link to clone: https://github.com/zancrash/CP386-A4.git

## Screenshots

## Individual Contribution (Function-wise)
Nicolas: safety(), main()
Farzan: main(), realFile(), runThread(), getSinglePointer(), getDoublePointer();

### view contribution list

## Features
int **readFile(char* filename);
void *runThread(void *t);
int *safetyCheck();
void getSinglePointer(int *info, int j);
void getDoublePointer(int **info, int j, int k);
void request(int resourceCurrent, int customerCount, int **need, int **allocated, int **max, int safe, char *inputString);
void release(int resourceCurrent, int customerCount, int **need, int **allocated, int **max, int safe);
void star(int resourceCurrent, int customerCount, int *available, int **needs, int **allocated, int **max);
void run(int safe, int customerCount);
void exit(int *available, int **max, int **allocated, int **needs, int *safetySequence);

## Tests

## Code Example

## Authors

(https://github.com/zancrash)

(https://github.com/Xlox88)

## Credits
(https://github.com/zancrash) Contributer

(https://github.com/Xlox88) Contributer


## License
