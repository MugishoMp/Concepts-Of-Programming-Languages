/**
 * @file expressionChecker.h
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

jmp_buf env2;

void checkExpression(char **expression) {

    // read expression string 
    TokenString * tokenString = createTokenString(*expression);
    // try-catch block
    if (setjmp(env2) == 0) {
        // tokenString->printExpression(tokenString);

        // create a string of tokens form the original expression
        tokenString->getTokenString(tokenString);
        // tokenString->printTokenString(tokenString);

        // // put this string of tokens into a lexer
        Lexer *lexer = createLexer(tokenString); 
        destroyLexer(lexer);
        // // do lexical analysis on this string of tokens
        // // and give it an empty parse tree to fill
        // ParseTree * parseTree = new ParseTree();

        // // and build it while doing the lexical analysis
        // l->expr(parseTree, nullptr);
        // // parseTree->print();
        
        destroyTokenString(tokenString);
    } else {
        printf("Error occurred. Cleaning up token string before exitintg\n");
        destroyTokenString(tokenString);
        return;
    }
    
    return;
}