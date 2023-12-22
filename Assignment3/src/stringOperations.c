/**
 * @file stringOperations.c
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

 #include <stdlib.h>
 #include <stringOperations.h>


void appendToString(char *string, size_t *stringLength, char character) {
    
    // pass the string reference and character to the function
    // Reallocate memory to accommodate the new character and null-terminator
    char *new_variable = realloc(string, *stringLength + 2);
    if (new_variable == NULL) {
        // Handle memory allocation error
        free(string);
        return;
    }
    string = new_variable;

    // Append the new character
    string[*stringLength] = character;
    *stringLength = *stringLength + 1;

    // Null-terminate the string
    string[*stringLength] = '\0';
}


