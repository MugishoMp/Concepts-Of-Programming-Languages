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

jmp_buf tokenStringException;
jmp_buf memoryAllocationException;
jmp_buf lexerException;

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