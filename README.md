# CP386-A4

## Motivation
To demostrate knowledge of banker's algorithm by implementing it in a C program.

## Installation
To run the banker's algorithm program, the repository must be cloned and its destination must be accessed from cmd to run.
Link to clone: https://github.com/zancrash/CP386-A4.git

After cloning, ensure the Makefile is in the root directory, then run:
  ```bash
  make
  ```

To execute the compiled program, run the file name, followed by the number of resources:

  ```base
  ./Question1 10 5 7 8
  ```

## Screenshots
![image](https://i.imgur.com/jjin7P9.png)

## Individual Contribution (Function-wise)

* Nicolas Lozano: main(), safety()
* Muhammad Ali: main(), realFile(), runThread(), getSinglePointer(), getDoublePointer();

### view contribution history for full overview

## Features

* RQ - Request resources to fill allocation array
* RL - Release Resources and update data structures
* Status - Display current state
* Run - Exectute threads in a safe sequence 
* Exit - Exit out of program

## Tests

See screenshots

## Code Example

```
// Function declarations
int **readFile(char* filename);
void *runThread(void *t);
int *safetyCheck();
void getSinglePointer(int *info, int j);
void getDoublePointer(int **info, int j, int k);
void run(int safe, int customerCount);
```
## Authors

Muhammad Ali: https://github.com/zancrash

Nicolas Lozano: https://github.com/Xlox88

## Credits

Contributor: Muhammad Ali: https://github.com/zancrash

Contributor: Nicolas Lozano: https://github.com/Xlox88


## License

Copyright 2021 Muhammad Ali and Nicolas Lozano