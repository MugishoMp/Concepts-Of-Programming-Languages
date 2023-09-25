/**
 * @file main.cc
 * @author Mugisho Mpozi s2440547
 * @brief main entry point
 * @version 0.1
 * @date 2023-9-25
 * 
 */

 #include <stdio.h>
 #include "custom_headers.h"

 /**
 * @brief Reads input from a file.
 *
 * This function reads lines from a file specified by @p readFileName and stores
 * them in the character array pointed to by @p expression.
 *
 * @param expression Pointer to a character array where the read input will be 
 * stored. This pointer will be allocated dynamically.
 * @param readFileName Name of the file to read from.
 *
 * @return Returns true if the file was successfully opened and read, false 
 * otherwise.
 */
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

int main(int argc, char* argv[]) {
    // if not exactly one input file has been specified exit program
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    } 
    
    // Program should:
    // 1. Read in an expression from standard input into a character string
    char *expression;
    if (readInput(&expression, argv[1])) {
        printf("Contents of %s:\n%s", argv[1], expression);
        free(expression);
    }
    // 2. Lexically analyzes the character strin into a string of tokens
    // 3. Parses the token string
    // 4. Outputs a character stirn gin a standard format to standard output

    return 0;
}