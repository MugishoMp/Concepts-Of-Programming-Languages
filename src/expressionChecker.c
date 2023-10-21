/**
 * @file expressionChecker.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

#include <stdio.h>
#include "tokenString.h"
#include "errorHandling.h"

jmp_buf env2;

void checkExpression(char **expression) {

    // try-catch block
    if (setjmp(env2) == 0) {
        // read expression string 
        TokenString * tokenString = createTokenString(*expression);
        tokenString->printExpression(tokenString);

        // create a string of tokens form the original expression
        tokenString->getTokenString(tokenString);
        tokenString->printTokenString(tokenString);

        // // put this string of tokens into a lexer
        // Lexer *l = new Lexer(tokenString); 

        // // do lexical analysis on this string of tokens
        // // and give it an empty parse tree to fill
        // ParseTree * parseTree = new ParseTree();

        // // and build it while doing the lexical analysis
        // l->expr(parseTree, nullptr);
        // // parseTree->print();

        // AbstractSyntaxTree * abstractSyntaxTree = new AbstractSyntaxTree(parseTree);
        // // abstractSyntaxTree->print(abstractSyntaxTree->getRoot());
        // abstractSyntaxTree->reduce();
        // abstractSyntaxTree->print(abstractSyntaxTree->getRoot());
        // // abstractSyntaxTree->print();

        // // keep track wich expression is wrong in a multi expression file
        // errorLine++;
        
        // cleanupTokenString(*tokenString);
    } else {
        printf("Error occurred. Cleaning up token string before exitintg\n");
        // cleanupTokenString(*tokenString);
        return;
    }
    
    return;
}