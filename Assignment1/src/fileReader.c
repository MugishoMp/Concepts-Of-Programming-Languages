/**
 * @file fileReader.c
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include "custom_headers.h"
 #include "fileReader.h"

bool readInput(char **expression, const char *readFileName) {
    FILE *inputFile = fopen(readFileName, "r");
    // check if the file was actually opened. Otherwise stop program
    if (inputFile == NULL) {
        printf("Unable to open file.\n");
        return false;
    } 
    // put the contents of the file into a string

    // find out the size of the file
    fseek(inputFile, 0, SEEK_END); // Move file pointer to the end of the file
    long fileSize = ftell(inputFile); // Get the size of the file
    fseek(inputFile, 0, SEEK_SET); // Reset file pointer to the beginning
    
    *expression = (char*)malloc(fileSize + 1); // Allocate memory for the string
    if (*expression == NULL) {
        fclose(inputFile);
        perror("Memory allocation error");
        return false;
    }

    size_t index = 0;
    int ch;

    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch != '\n') {
            (*expression)[index++] = (char)ch;
        }
    }

    (*expression)[fileSize] = '\0'; // Add null terminator to indicate end of string

    fclose(inputFile);
    return true;
}