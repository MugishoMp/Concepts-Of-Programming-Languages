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


void expr(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);
    Node * node = malloc(sizeof(Node));
    node->info.expression = "EXPR";
    node->info.string = " ";
    node->parent = parentNode;
    parseTree->addNode(node, parseTree);

    this->lexpr(parseTree, node, this);
    this->expr1(parseTree, node, this);
}

void expr1(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);
    Node * node = malloc(sizeof(Node));
    node->info.expression = "EXPR1";
    node->info.string = " ";
    node->parent = parentNode;
    parseTree->addNode(node, parseTree);

    if (!(this->isEmpty(this)) && !(strcmp(this->peek(this)->token, "BRACKET_CLOSE") && this->numberOfOpenBrackets > 0)) {
        this->lexpr(parseTree, node, this);
        this->expr1(parseTree, node, this);
    }
}

void lexpr(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);

}

void pexpr(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);

}

bool isEmpty(void* self) {
    Lexer * this = ((Lexer*)self);
    return ((unsigned int) this->index >= this->tokenString->getTokenString(this->tokenString)->size);

}

TokenStringPair * peek(void* self) {
    Lexer * this = ((Lexer*)self);
    TokenStringPair * token = this->tokenString->getTokenByIndex(this->index,this->tokenString);
    return token;
}

void consume(void* self) {
    Lexer * this = ((Lexer*)self);
    this->index++;
}


Lexer * createLexer(TokenString * tokenString) {
    Lexer * obj = malloc(sizeof(Lexer));
    if (obj == NULL) {
        longjmp(memoryAllocationException, 1);
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

        obj->expr = expr;
        obj->expr1 = expr1;
        obj->lexpr = lexpr;
        obj->pexpr = pexpr;
        obj->isEmpty = isEmpty;
        obj->peek = peek;
        obj->consume = consume;

    return obj;
}

void destroyLexer(Lexer *obj) {
    if (obj == NULL) {
        return;  // Nothing to free
    }
    destroyTokenString(obj->getTokenString(obj));
    // Free the TokenString object itself
    free(obj);
}