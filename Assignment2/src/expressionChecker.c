/**
 * @file expressionChecker.c
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

#include <stdio.h>
#include "lexer.h"
#include "tokenString.h"
#include "errorHandling.h"
#include "expressionChecker.h"

jmp_buf tokenStringException;
jmp_buf memoryAllocationException;
jmp_buf lexerException;


/* Function to check if a character is a whitespace character */
int isWhitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

/* Function to check if char **expression is empty or only contains whitespace characters */
int isEmptyExpression(char *expression) {
    if (expression == NULL) {
        return 1; // Consider NULL as empty
    }

    for (int i = 0; expression[i] != '\0'; i++) {
        if (!isWhitespace(expression[i])) {
            return 0; // Found a non-whitespace character
        }
    }

    return 1; // String is empty or contains only whitespace
}


void checkExpression(char **expression) {

    if (setjmp(tokenStringException) == 0) {
        // read expression string 
        TokenString * tokenString = createTokenString(*expression);
        // try-catch block
        if (setjmp(tokenStringException) == 0) {
            // create a string of tokens form the original expression
            tokenString->getTokenString(tokenString);
            // tokenString->printTokenString(tokenString);

        } else {
            printf("Error occurred::TokenString\n");
            destroyTokenString(tokenString);
            return;
        }

        // and give it an empty parse tree to fill
        ParseTree * parseTree = createParseTree();
        // put this string of tokens into a lexer
        Lexer *lexer = createLexer(tokenString); 

        if (setjmp(lexerException) == 0) {

            // // build parse tree top down while doing the lexical analysis
            lexer->expr(parseTree, NULL, lexer);
            // // parseTree->print();
            parseTree->printParseTree(parseTree);
        } else {
            printf("Error occurred::Lexical Analysis\n");
            destroyParseTree(parseTree);
            destroyLexer(lexer);
            return;
        }

        // destroyLexer will also take care of destroying the tokenString
        destroyLexer(lexer);
        destroyParseTree(parseTree);
    } else {
        printf("Error occurred. problem allocating memory\n");
        return;
    }

    return;
}
