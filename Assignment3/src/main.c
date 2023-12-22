/**
 * @file main.cc
 * @author Mugisho Mpozi s2440547
 * @brief main entry point
 * @version 0.1
 * @date 2023-9-25
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileReader.h"
#include "expressionChecker.h"
#include "errorHandling.h"


jmp_buf env;

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

        // try-catch block
        char *subExpression;
    
        // Initialize strtok and get the first subExpression
        subExpression = strtok(expression, "\n");
        
        // Loop through to get each sub-expression
        while (subExpression != NULL) {
            
            if (setjmp(env) == 0) {
                // check expression per line
                checkExpression(&subExpression);
            } else {
                // an error occurred
                free(subExpression);
                return 1;
            }

            // Get the next subExpression
            subExpression = strtok(NULL, "\n");
        }


        free(expression);
    } else {
        printf("Could not read input");
        return 1;
    }
    
    return 0;
}