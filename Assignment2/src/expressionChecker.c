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
#include "abstractSyntaxTree.h"

jmp_buf tokenStringException;
jmp_buf memoryAllocationException;
jmp_buf lexerException;

void checkExpression(char **expression) {

    //expression check try catch block
    if (setjmp(tokenStringException) == 0) {
        // TOKEN STRING
        TokenString * tokenString = createTokenString(*expression);

        //token string try catch block
        if (setjmp(tokenStringException) == 0) {
            // create a string of tokens form the original expression
            tokenString->getTokenString(tokenString);
            // tokenString->printTokenString(tokenString);

        } else {
            printf("Error occurred::TokenString\n");
            destroyTokenString(tokenString);
            return;
        }



        // LEXER
        // and give it an empty parse tree to fill
        ParseTree * parseTree = createParseTree();
        // put this string of tokens into a lexer
        Lexer *lexer = createLexer(tokenString); 

        if (setjmp(lexerException) == 0) {

            // build parse tree top down while doing the lexical analysis
            lexer->expr(parseTree, NULL, lexer);
            // parseTree->printParseTree(parseTree);
            // parseTree->printDisambiguatedExpression(parseTree);
            AbstractSyntaxTree * abstractSyntaxTree = createAbstractSyntaxTree(parseTree);
            abstractSyntaxTree->cleanUpTree(abstractSyntaxTree);
            abstractSyntaxTree->print(abstractSyntaxTree->getRoot(abstractSyntaxTree), abstractSyntaxTree);

            printf("\n");
        } else {
            printf("Error occurred::Lexical Analysis\n");
            destroyParseTree(parseTree);
            destroyLexer(lexer);
            return;
        }



        // CLEANUP
        // destroyLexer will also take care of destroying the tokenString
        destroyLexer(lexer);
        destroyParseTree(parseTree);
    } else {
        printf("Error occurred. problem allocating memory\n");
        return;
    }

    return;
}