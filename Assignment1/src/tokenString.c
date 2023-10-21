/**
 * @file tokenString.c
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenString.h"
#include "stringOperations.h"
#include "errorHandling.h"

// oop
// attributes
// getters
// setters
// read function
// print function
char * getExpression(void* self) {
    return ((TokenString*)self)->expression;
}

void setExpression(char * expression, void* self) {
    ((TokenString*)self)->expression = expression;
}

int getIndex(void* self) {
    return ((TokenString*)self)->index;
}

void setIndex(int index, void* self) {
    ((TokenString*)self)->index = index;
}

TokenCharacterPairArray * getTokenString(void* self) {
    // if the tokenstring is not yet initialized, create this tokenstring using 
    // the expression. So essentially thin functino "updates" our tokenstring
    // in a way.
    if(((TokenString*)self)->tokenString->size == 0) {
        ((TokenString*)self)->tokenize((TokenString*)self);
    }
    // else, return that field
    return ((TokenString*)self)->tokenString;
}




// Function to append a TokenStringPair to a TokenCharacterPairArray
// Returns 0 if successful, and -1 if memory allocation fails
void appendTokenStringPair(TokenStringPair *pair, void* self) {

    // printf("token to be added: (%s, %s)", pair->token, pair->character);
    // Reallocate memory to accommodate the new TokenStringPair
    TokenCharacterPairArray * array = ((TokenString*)self)->tokenString;
    TokenStringPair **new_array = realloc(array->array, (array->size + 1) * sizeof(TokenStringPair *));

    if (new_array == NULL) {
        return;  // Memory allocation failed
    }
    array->array = new_array;
    
    // Add the new TokenStringPair
    array->array[array->size] = pair;
    // printf("token to be added: (%s, %s). Size: %d \n", array->array[array->size]->token, array->array[array->size]->character, ((TokenString*)self)->tokenString->size);
    array->size++;
    

    return;  // Successful append
}

void tokenize(void* self) {
    char *expression = getExpression((TokenString*)self);
    for (int i = 0; i < (int)strlen(expression); i++){
        
        switch (expression[i]) {
            case ' ':
                break;
            case '(': {
                TokenStringPair *token = malloc(sizeof(TokenStringPair));
                if (token != NULL) {
                    token->token = "BRACKET_OPEN";
                    token->character = "(";
                }
                // add to tokenstring
                appendTokenStringPair(token, ((TokenString*)self));
                break;
            }
            case ')': {
                TokenStringPair *token = malloc(sizeof(TokenStringPair));
                if (token != NULL) {
                    token->token = "BRACKET_CLOSE";
                    token->character = ")";
                }
                appendTokenStringPair(token, ((TokenString*)self));
                break;
            }
            case '\\': {
                TokenStringPair *token = malloc(sizeof(TokenStringPair));
                if (token != NULL) {
                    token->token = "BACKSLASH";
                    token->character = "\\";
                }
                appendTokenStringPair(token, ((TokenString*)self));
                break;
            }
            default: {
                TokenStringPair * token = ((TokenString*)self)->tokenizeVariable(&i, ((TokenString*)self));
                if (token->character == NULL) {
                    printf("Error::TokenString::tokenizeVariable: ");
                    printf("Could not tokenize expression");
                }
                appendTokenStringPair(token, ((TokenString*)self));
            }
        }
    }
}

TokenStringPair * tokenizeVariable(int *i, void* self) {
    char *expression = getExpression((TokenString*)self);
    if (!(expression[*i]  >= 'A' && expression[*i] <= 'Z') 
        && !(expression[*i]  >= 'a' && expression[*i] <= 'z')
        && !(expression[*i]  >= '0' && expression[*i] <= '9')) {
        printf("Error::TokenString::tokenizeVariable: ");
        printf("Variable not starting with letter from latin alphabet");
        longjmp(env2, 1);
    }

    // Initialize an empty string
    char *variableString = malloc(1);  // Allocate space for the null-terminator
    if (variableString == NULL) {
        // Handle memory allocation error
        printf("Error::TokenString::tokenizeVariable: ");
        printf("Could not allocate space for variable string");
        longjmp(env2, 1);
    }
    variableString[0] = '\0';  // Null-terminate the string
    size_t len = 0;  // Current length of the string

    while ((expression[*i]  >= 'A' && expression[*i] <= 'Z') 
        || (expression[*i]  >= 'a' && expression[*i] <= 'z')
        || (expression[*i]  >= '0' && expression[*i] <= '9')) {

        appendToString(variableString, &len, expression[*i]);
        
        *i += 1;
    }

    *i -= 1;
    TokenStringPair *token = malloc(sizeof(TokenStringPair));
    if (token != NULL) {
        token->token = "VARIABLE";
        token->character = variableString;
    }
    return token;
}

void printExpression(void* self) {
    printf("Expression: %s \n", ((TokenString*)self)->expression);
}

void printTokenString(void* self) {
    // Use the strings
    printf("Token String: ");
    for (int i = 0; i < getTokenString((TokenString*)self)->size; i++) {
        printf("%s ", getTokenString((TokenString*)self)->array[i]->token);
    }
    printf("\n");
}

TokenString * createTokenString(char * expression) {
    TokenString * obj = malloc(sizeof(TokenString));
    if (obj == NULL) {
        return NULL;
    }

    // fields
        obj->index = 0;
        obj->expression = expression;
        
        obj->tokenString = malloc(sizeof(TokenCharacterPairArray));
        obj->tokenString->size = 0;
        obj->tokenString->array = malloc(obj->tokenString->size * sizeof(char *));

    // functions
        // ## getters and setters
        obj->setExpression = setExpression;
        obj->getExpression = getExpression;
        obj->getTokenString = getTokenString;

        // 
        obj->tokenize = tokenize;
        obj->tokenizeVariable = tokenizeVariable;
        obj->appendTokenStringPair = appendTokenStringPair;

        // ## print functions
        obj->printExpression = printExpression;
        obj->printTokenString = printTokenString;

    return obj;
}

void destroyTokenString(TokenString *obj) {
    if (obj == NULL) {
        return;  // Nothing to free
    }

    // Free each TokenStringPair
    for (int i = 0; i < obj->tokenString->size - 3; ++i) {
        // Assume that the token and character strings in each TokenStringPair
        // were dynamically allocated; if not, remove these lines

        // Only the character in dynamically allocated, and only in the case
        // for the variable token
        if (strcmp(obj->tokenString->array[i]->token, "VARIABLE") == 0) {
            // if token == VARIABLE
            free(obj->tokenString->array[i]->character);
        }

        // Free the TokenStringPair itself
        free(obj->tokenString->array[i]);
    }

    // Free the array of TokenStringPairs
    free(obj->tokenString->array);

    // Free the TokenCharacterPairArray
    free(obj->tokenString);

    // Free the TokenString object itself
    free(obj);
}