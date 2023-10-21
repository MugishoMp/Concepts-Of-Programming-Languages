/**
 * @file lexer.c
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "errorHandling.h"
#include <tokenString.h>



int getIndexLexer(void* self) {
    return ((Lexer*)self)->index;
}

void setIndexLexer(int index, void* self) {
    ((Lexer*)self)->index = index;
}

int getNumberOfOpenBracketsLexer(void* self) {
    return ((Lexer*)self)->numberOfOpenBrackets;
}

void setNumberOfOpenBracketsLexer(int numberOfOpenBrackets, void* self) {
    ((Lexer*)self)->numberOfOpenBrackets = numberOfOpenBrackets;
}

TokenString * getTokenStringLexer(void* self) {
    return ((Lexer*)self)->tokenString;
}

void setTokenStringLexer(TokenString * tokenString, void* self) {
    ((Lexer*)self)->tokenString = tokenString;
}

Lexer * createLexer(TokenString * tokenString) {
    Lexer * obj = malloc(sizeof(TokenString));
    if (obj == NULL) {
        return NULL;
    }

    // fields
        obj->index = 0;
        obj->numberOfOpenBrackets = 0;
        obj->tokenString = tokenString;

    // functions
        obj->getIndex = getIndexLexer;
        obj->setIndex = setIndexLexer;
        obj->getNumberOfOpenBrackets = getNumberOfOpenBracketsLexer;
        obj->setNumberOfOpenBrackets = setNumberOfOpenBracketsLexer;
        obj->getTokenString = getTokenStringLexer;
        obj->setTokenString = setTokenStringLexer;

    return obj;
}

void destroyLexer(Lexer *obj) {
    // Free the TokenString object itself
    free(obj);
}