/**
 * @file lexer.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 */

#ifndef LEXER_H
#define LEXER_H

#include <tokenString.h>
#include <node.h>
#include <parseTree.h>
#include <custom_headers.h>

typedef struct {
    int index;
    int numberOfOpenBrackets;
    TokenString * tokenString;

    int (*getIndex)(void*);
    void (*setIndex)(int, void*);
    int (*getNumberOfOpenBrackets)(void*);
    void (*setNumberOfOpenBrackets)(int, void*);
    TokenString * (*getTokenString)(void*);
    void (*setTokenString)(TokenString *, void*);

    void (*expr) (ParseTree *, Node *, void*);
    void (*expr1) (ParseTree *, Node *, void*);
    void (*lexpr) (ParseTree *, Node *, void*);
    void (*pexpr) (ParseTree *, Node *, void*);
    bool (*isEmpty) (void*);
    TokenStringPair * (*peek) (void*);
    void (*consume) (void*);


} Lexer;

// constructor 
Lexer * createLexer(TokenString * tokenString);
// destructor
void destroyLexer(Lexer * obj);

#endif // LEXER_H
