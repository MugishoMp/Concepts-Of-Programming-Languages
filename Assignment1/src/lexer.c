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
    // printf("test: %s\n", parseTree->getRoot(parseTree)->info.expression);
    // parseTree->printParseTree(parseTree);
    this->lexpr(parseTree, node, this);
    this->expr1(parseTree, node, this);
}

void expr1(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);
    Node * node = malloc(sizeof(Node));
    node->info.expression = "EXPR1";
    node->info.string = "";
    node->parent = parentNode;
    parseTree->addNode(node, parseTree);
    // parseTree->printParseTree(parseTree);

    if (!(this->isEmpty(this)) && !(strcmp(this->peek(this)->token, "BRACKET_CLOSE") == 0 && this->numberOfOpenBrackets > 0)) {
        this->lexpr(parseTree, node, this);
        this->expr1(parseTree, node, this);
    }
}

void lexpr(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);
    Node * node = malloc(sizeof(Node));
    node->info.expression = "LEXPR";
    node->info.string = "";
    node->parent = parentNode;
    parseTree->addNode(node, parseTree);
    // parseTree->printParseTree(parseTree);

    if (!(this->isEmpty(this)) && (strcmp(this->peek(this)->token, "BACKSLASH") == 0)) {
        this->consume(this);
        if (!(this->isEmpty(this)) && (strcmp(this->peek(this)->token, "VARIABLE") == 0)) {
            Node * node2 = malloc(sizeof(Node));
            node2->info.expression = "BACKSLASH";
            char *string = malloc(strlen(this->peek(this)->character) + 1);
            if (string == NULL) {
                longjmp(memoryAllocationException, 1);
            }
            strcpy(string, this->peek(this)->character);
            node2->info.string = string;
            node2->parent = parentNode;
            parseTree->addNode(node2, parseTree);
            // parseTree->printParseTree(parseTree);

            this->consume(this);

            if (this->isEmpty(this)) {
                printf("Lexer::Error:: Missing expression after lamda abstraction\n");
                longjmp(lexerException, 1);
            }
            this->lexpr(parseTree, node, this);
        } else {
            printf("Lexer::Error:: Missing variable after lambda\n");
            longjmp(lexerException, 1);
        }

    } else {
        this->pexpr(parseTree, node, this);
    }
}

void pexpr(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);
    Node * node = malloc(sizeof(Node));
    node->info.expression = "PEXPR";
    node->info.string = "";
    node->parent = parentNode;
    parseTree->addNode(node, parseTree);
    // parseTree->printParseTree(parseTree);

    // printf("pexpr: %s", this->peek(this)->token);
    
    if (!(this->isEmpty(this)) && (strcmp(this->peek(this)->token, "BRACKET_OPEN") == 0)) {
        this->consume(this);
        this->numberOfOpenBrackets++;

        if (this->isEmpty(this) || (!(this->isEmpty(this)) && (strcmp(this->peek(this)->token, "BRACKET_CLOSE") == 0))) {
            printf("Lexer::Error:: Missing expression after opening parenthesis\n");
            longjmp(lexerException, 1);
        }

        this->expr(parseTree, node, this);

        if (!(this->isEmpty(this)) && (strcmp(this->peek(this)->token, "BRACKET_CLOSE") == 0)) {
            this->consume(this);
            this->numberOfOpenBrackets--;
        } else {
            printf("Lexer::Error:: Missing closing parenthesis\n");
            longjmp(lexerException, 1);
        }


    } else if (!(this->isEmpty(this)) && (strcmp(this->peek(this)->token, "VARIABLE") == 0)) {
        Node * node2 = malloc(sizeof(Node));
        node2->info.expression = "PEXPR";
        char *string = malloc(strlen(this->peek(this)->character) + 1);
        if (string == NULL) {
            longjmp(memoryAllocationException, 1);
        }
        strcpy(string, this->peek(this)->character);
        node2->info.string = string;
        node2->parent = parentNode;
        parseTree->addNode(node2, parseTree);
        // parseTree->printParseTree(parseTree);
        this->consume(this);
    } else {
        printf("Lexer::Error:: Missing variable\n");
        longjmp(lexerException, 1);
    }

}

bool isEmpty(void* self) {
    Lexer * this = ((Lexer*)self);
    if (this->index >= this->tokenString->getTokenString(this->tokenString)->size)
        return true;
    else 
        return false;

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