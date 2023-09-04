/*=============================================================================
| Assignment: pa02 - Calculating an 8, 16, or 32 bit
| checksum on an ASCII input file
|
| Author: Kirk Lefevre
| Language: C
|
| To Compile: javac pa02.java
| gcc -o pa02 pa02.c
| g++ -o pa02 pa02.cpp
| go build pa02.go
| python pa02.py //Caution - expecting input parameters
|
| To Execute: java -> java pa02 inputFile.txt 8
| or c++ -> ./pa02 inputFile.txt 8
| or c -> ./pa02 inputFile.txt 8
| or go -> ./pa02 inputFile.txt 8
| or python-> python pa02.py inputFile.txt 8
| where inputFile.txt is an ASCII input file
| and the number 8 could also be 16 or 32
| which are the valid checksum sizes, all
| other values are rejected with an error message
| and program termination
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Spring 2023
| Instructor: McAlpin
| Due Date: per assignment
|
+=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_INPUT_SIZE 100000

void printUsage() {
    fprintf(stderr, "Usage: checksum [input file name] [checksum size]\n");
    fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
}

int main(int argc, char *argv[]) {
    // Check the number of command line arguments
    if (argc != 3) {
        printUsage();
        return 1;
    }
    
    // Parse the input file name and checksum size from command line arguments
    char* inputFileName = argv[1];
    int checksumSize = atoi(argv[2]);
    
    // Check if the checksum size is valid
    if (checksumSize != 8 && checksumSize != 16 && checksumSize != 32) {
        printUsage();
        return 1;
    }
    
    // Open the input file
    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", inputFileName);
        return 1;
    }
    
    // Read the input file into a buffer
    char inputBuffer[MAX_INPUT_SIZE];
    int inputSize = 0;
    int inputChar;
    while ((inputChar = fgetc(inputFile)) != EOF) {
        inputBuffer[inputSize++] = (char) inputChar;
    }
    inputBuffer[inputSize+1] = '\0';
    
    // Echo the input text to the console (STDOUT) in rows of 80 characters per line
    int i;
    for (i = 0; i < inputSize; i += 80) {
        printf("\n%.80s", inputBuffer + i);
    }
    
    // Calculate the checksum
    unsigned long checksum = 0;
    for (i = 0; i < inputSize; i++) {
        checksum += (unsigned char) inputBuffer[i];
    }
    checksum = checksum%(unsigned int)pow(2,8);
    
    // Pad the input data with X if necessary to align with checksum size
    int padding = checksumSize / 8 - inputSize % (checksumSize / 8);
    if (padding == checksumSize / 8) {
        padding = 0;
    }
    for (i = 0; i < padding; i++) {
        fprintf(stdout, "X");
        inputBuffer[inputSize + i] = 'X';
    }
    inputSize += padding;
    
    // Calculate the 16-bit and 32-bit checksums if necessary
    if (checksumSize == 16) {
        int j;
        checksum = 0;
        for (i = 0; i < inputSize - inputSize%2; i += 2) {
            unsigned short word = ((unsigned char) inputBuffer[i] << 8) | (unsigned char) inputBuffer[i+1];
            checksum += (unsigned long) word;
        }
        checksum %=(unsigned int)(pow(2,16)/2);
    }
    else if (checksumSize == 32) {
        checksum = 0;
        for(i = 0; i<inputSize; i+=4){
            u_int32_t value = ((u_int32_t)inputBuffer[i] << 24) |
                        ((u_int32_t)inputBuffer[i + 1] << 16) |
                        ((u_int32_t)inputBuffer[i + 2] << 8) |
                        (u_int32_t)inputBuffer[i + 3];
            checksum += (unsigned long) value;
        }
        if (i != inputSize) { // Check if there are any remaining bytes
            u_int32_t value = 0;
            size_t remainingBytes = inputSize - i;
            size_t j;

            for (j = 0; j < remainingBytes; ++j) {
                value |= ((u_int32_t)inputBuffer[i + j] << (24 - (8 * j)));
            }

            checksum += value;
        }

        unsigned int x = (checksum%(unsigned int)pow(2,32) - checksum);
        checksum-=x;

        checksum %= (unsigned int)(pow(2,32));
        
        
    }
    
    
    // Print the checksum character count to the console
    fprintf(stdout, "\n%d bit checksum is \t%0*lx for all %d chars\n\n", checksumSize, checksumSize / 4, checksum, inputSize);
    return 0;
}

/*=============================================================================
|       I Kirk Lefevre (ki689220) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+============================================================================*/

// not saying this for brownie points, but thank you for being a wonderful professor. Your lectures were
// always entertaining, and i loved coming to your class. If i could take you again, I absolutely would
// for one of my future electives. Thank you again, and have a wonderful summer!