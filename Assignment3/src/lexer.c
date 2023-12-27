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

static int getIndexLexer(void* self);
static void setIndexLexer(int index, void* self);
static int getNumberOfOpenBracketsLexer(void* self);
static void setNumberOfOpenBracketsLexer(int numberOfOpenBrackets, void* self);
static TokenString * getTokenStringLexer(void* self);
static void setTokenStringLexer(TokenString * tokenString, void* self);
static void judgement(ParseTree * parseTree, Node * parentNode, void* self);
static void expr(ParseTree * parseTree, Node * parentNode, void* self);
static void expr1(ParseTree * parseTree, Node * parentNode, void* self);
static void lexpr(ParseTree * parseTree, Node * parentNode, void* self);
static void pexpr(ParseTree * parseTree, Node * parentNode, void* self);
static void type(ParseTree * parseTree, Node * parentNode, void* self);
static void type1(ParseTree * parseTree, Node * parentNode, void* self);
static void ptype(ParseTree * parseTree, Node * parentNode, void* self);
static bool isEmpty(void* self);
static TokenStringPair * peek(void* self);
static void consume(void* self);




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
        obj->judgement = judgement;

    return obj;
}

void destroyLexer(Lexer *obj) {
    if (obj == NULL) {
        return;  // Nothing to free
    }
    destroyTokenString(obj->tokenString);
    // Free the TokenString object itself
    free(obj);
}


static int getIndexLexer(void* self) {
    return ((Lexer*)self)->index;
}

static void setIndexLexer(int index, void* self) {
    ((Lexer*)self)->index = index;
}

static int getNumberOfOpenBracketsLexer(void* self) {
    return ((Lexer*)self)->numberOfOpenBrackets;
}

static void setNumberOfOpenBracketsLexer(int numberOfOpenBrackets, void* self) {
    ((Lexer*)self)->numberOfOpenBrackets = numberOfOpenBrackets;
}

static TokenString * getTokenStringLexer(void* self) {
    return ((Lexer*)self)->tokenString;
}

static void setTokenStringLexer(TokenString * tokenString, void* self) {
    ((Lexer*)self)->tokenString = tokenString;
}


// ⟨judgement⟩ ::= ⟨expr⟩ ':' ⟨type⟩
static void judgement(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);

    Node * node = malloc(sizeof(Node));
    node->info.expression = "JUDGEMENT";
    node->info.string = ":";
    node->parent = parentNode;
    node->singleChild = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;

    parseTree->addNode(node, parseTree);

    expr(parseTree, node, this);

    if (!(isEmpty(this)) && (strcmp(peek(this)->token, "COLON") == 0)) {
        consume(this);
    } else {
        printf("Lexer::Error:: Missing colom\n");
        longjmp(lexerException, 1);
    }

    type(parseTree, node, this);
    // printf("number of unchecked brackets: %d", this->numberOfOpenBrackets);
}

//〈expr〉 ::= 〈lexpr〉〈expr′〉 
static void expr(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);

    Node * node = malloc(sizeof(Node));
    node->info.expression = "EXPR";
    node->info.string = "";
    node->parent = parentNode;
    node->singleChild = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;

    parseTree->addNode(node, parseTree);

    lexpr(parseTree, node, this);
    expr1(parseTree, node, this);
}



//〈expr′〉 ::= 〈lexpr〉〈expr′〉 | e 
static void expr1(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);

    Node * node = malloc(sizeof(Node));
    node->info.expression = "EXPR1";
    node->info.string = "";
    node->parent = parentNode;
    node->singleChild = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;
    
    parseTree->addNode(node, parseTree);
    // parseTree->printParseTree(parseTree);

    if (!(isEmpty(this)) && 
        !(strcmp(peek(this)->token, "BRACKET_CLOSE") == 0 && this->numberOfOpenBrackets > 0) && 
        !(strcmp(peek(this)->token, "COLON") == 0)) 
    {
        lexpr(parseTree, node, this);
        expr1(parseTree, node, this);
    }
}


//〈lexpr〉 ::= 〈pexpr〉 | '\' ⟨lvar⟩ '^' ⟨type⟩ ⟨lexpr⟩ {
static void lexpr(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);

    Node * node = malloc(sizeof(Node));
    node->info.expression = "LEXPR";
    node->info.string = "";
    node->parent = parentNode;
    node->singleChild = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;

    parseTree->addNode(node, parseTree);
    // parseTree->printParseTree(parseTree);

    if (!(isEmpty(this)) && (strcmp(peek(this)->token, "BACKSLASH") == 0)) {
        consume(this);
        if (!(isEmpty(this)) && (strcmp(peek(this)->token, "LVAR") == 0)) {
            Node * node2 = malloc(sizeof(Node));
            node2->info.expression = "BACKSLASH";
            char *string = malloc(strlen(peek(this)->character) + 1);
            if (string == NULL) {
                longjmp(memoryAllocationException, 1);
            }
            strcpy(string, peek(this)->character);
            node2->info.string = string;
            node2->parent = node;
            node2->singleChild = NULL;
            node2->leftChild = NULL;
            node2->rightChild = NULL;
            parseTree->addNode(node2, parseTree);

            consume(this);

            
            if ((isEmpty(this)) || (strcmp(peek(this)->token, "ROOF") != 0)) {
                printf("Lexer::Error:: Missing roof symbol\n");
                longjmp(lexerException, 1);
            }
            consume(this);

            if (isEmpty(this)) {
                printf("Lexer::Error:: Missing type after lambda abstraction\n");
                longjmp(lexerException, 1);
            }
            type(parseTree, node2, this);

            if (isEmpty(this)) {
                printf("Lexer::Error:: Missing expression after lambda abstraction\n");
                longjmp(lexerException, 1);
            }
            lexpr(parseTree, node2, this);
        } else {
            printf("Lexer::Error:: Missing variable after lambda\n");
            longjmp(lexerException, 1);
        }

    } else {
        pexpr(parseTree, node, this);
    }
}

//〈pexpr〉 ::= 〈lvar〉 | (〈expr〉)
static void pexpr(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);
    Node * node = malloc(sizeof(Node));
    node->info.expression = "PEXPR";
    node->info.string = "";
    node->parent = parentNode;
    node->singleChild = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;
    parseTree->addNode(node, parseTree);
    // parseTree->printParseTree(parseTree);
    
    if (!(isEmpty(this)) && (strcmp(peek(this)->token, "BRACKET_OPEN") == 0)) {
        consume(this);
        this->numberOfOpenBrackets++;

        if (isEmpty(this) || (!(isEmpty(this)) && (strcmp(peek(this)->token, "BRACKET_CLOSE") == 0))) {
            printf("Lexer::Error:: Missing expression after opening parenthesis\n");
            longjmp(lexerException, 1);
        }

        expr(parseTree, node, this);

        if (!(isEmpty(this)) && (strcmp(peek(this)->token, "BRACKET_CLOSE") == 0)) {
            consume(this);
            this->numberOfOpenBrackets--;
        } else {
            printf("Lexer::Error:: Missing closing parenthesis\n");
            longjmp(lexerException, 1);
        }


    } else if (!(isEmpty(this)) && (strcmp(peek(this)->token, "LVAR") == 0)) {
        Node * node2 = malloc(sizeof(Node));
        node2->info.expression = "LVAR";
        char *string = malloc(strlen(peek(this)->character) + 1);
        if (string == NULL) {
            longjmp(memoryAllocationException, 1);
        }
        strcpy(string, peek(this)->character);
        node2->info.string = string;
        node2->parent = node;
        node2->singleChild = NULL;
        node2->leftChild = NULL;
        node2->rightChild = NULL;
        parseTree->addNode(node2, parseTree);
        consume(this);
    } else {
        printf("Lexer::Error:: Missing lower case variable\n");
        longjmp(lexerException, 1);
    }

}


// ⟨type⟩ ::= ⟨ptype⟩⟨type1⟩
static void type(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);

    Node * node = malloc(sizeof(Node));
    node->info.expression = "EXPR";
    node->info.string = "";
    node->parent = parentNode;
    node->singleChild = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;

    parseTree->addNode(node, parseTree);

    ptype(parseTree, node, this);
    type1(parseTree, node, this);
}

// ⟨type'⟩ ::= -> ⟨type⟩ | e
static void type1(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);

    Node * node = malloc(sizeof(Node));
    node->info.expression = "TYPE1";
    node->info.string = "";
    node->parent = parentNode;
    node->singleChild = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;
    
    parseTree->addNode(node, parseTree);


    if (!(isEmpty(this)) && 
        !(strcmp(peek(this)->token, "BRACKET_CLOSE") == 0 && this->numberOfOpenBrackets > 0) && 
        (strcmp(peek(this)->token, "ARROW") == 0)) {

        consume(this);
        type(parseTree, node, this);
    }
}

// ⟨ptype⟩ ::= ⟨uvar⟩ | '(' ⟨type⟩ ')'
static void ptype(ParseTree * parseTree, Node * parentNode, void* self) {
    Lexer * this = ((Lexer*)self);
    Node * node = malloc(sizeof(Node));
    node->info.expression = "PTYPE";
    node->info.string = "";
    node->parent = parentNode;
    node->singleChild = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;
    parseTree->addNode(node, parseTree);
    // parseTree->printParseTree(parseTree);
    
    if (!(isEmpty(this)) && (strcmp(peek(this)->token, "BRACKET_OPEN") == 0)) {
        consume(this);
        this->numberOfOpenBrackets++;

        if (isEmpty(this) || (!(isEmpty(this)) && (strcmp(peek(this)->token, "BRACKET_CLOSE") == 0))) {
            printf("Lexer::Error:: Missing expression after opening parenthesis\n");
            longjmp(lexerException, 1);
        }

        type(parseTree, node, this);

        if (!(isEmpty(this)) && (strcmp(peek(this)->token, "BRACKET_CLOSE") == 0)) {
            consume(this);
            this->numberOfOpenBrackets--;
        } else {
            printf("Lexer::Error:: Missing closing parenthesis\n");
            longjmp(lexerException, 1);
        }


    } else if (!(isEmpty(this)) && (strcmp(peek(this)->token, "UVAR") == 0)) {
        Node * node2 = malloc(sizeof(Node));
        node2->info.expression = "UVAR";
        char *string = malloc(strlen(peek(this)->character) + 1);
        if (string == NULL) {
            longjmp(memoryAllocationException, 1);
        }
        strcpy(string, peek(this)->character);
        node2->info.string = string;
        node2->parent = node;
        node2->singleChild = NULL;
        node2->leftChild = NULL;
        node2->rightChild = NULL;
        parseTree->addNode(node2, parseTree);
        consume(this);
    } else {
        printf("Lexer::Error:: Missing uppercase variable\n");
        longjmp(lexerException, 1);
    }

}

static bool isEmpty(void* self) {
    Lexer * this = ((Lexer*)self);
    if (this->index >= this->tokenString->getTokenString(this->tokenString)->size)
        return true;
    else 
        return false;

}

static TokenStringPair * peek(void* self) {
    Lexer * this = ((Lexer*)self);
    TokenStringPair * token = this->tokenString->getTokenByIndex(this->index, this->tokenString);
    return token;
}

static void consume(void* self) {
    Lexer * this = ((Lexer*)self);
    TokenStringPair * token = this->tokenString->getTokenByIndex(this->index, this->tokenString);
    // printf("variable: %s\n", token->token);
    this->index++;
    
}