/**
 * @file tokenString.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

#ifndef TOKENSTRING_H
#define TOKENSTRING_H

typedef struct {
    char *token;
    char *character;
} TokenStringPair;

typedef struct {
    TokenStringPair **array;
    int size;
} TokenCharacterPairArray;

typedef struct {
    char * expression;
    TokenCharacterPairArray * tokenString;
    int index;
    // tokenStrings

    int * (*getIndex)(void*);
    void (*setIndex)(int, void*);
    char * (*getExpression)(void*);
    void (*setExpression)(char *, void*);
    TokenCharacterPairArray * (*getTokenString)(void*);

    void (*appendTokenStringPair)(TokenStringPair *, void*);
    void (*tokenize)(void*);
    TokenStringPair * (*tokenizeVariable)(int *, void*);
    TokenStringPair * (*getTokenByIndex)(int, void*);

    void (*printExpression)(void*);
    void (*printTokenString)(void*);
} TokenString;

// constructor 
TokenString * createTokenString(char * expression);
// destructor
void destroyTokenString(TokenString *obj);

#endif // TOKENSTRING_H
