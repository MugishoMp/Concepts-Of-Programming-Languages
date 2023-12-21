/**
 * @file abstractSyntaxTree.c
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractSyntaxTree.h"
#include "errorHandling.h"
#include <tokenString.h>





#include "abstractSyntaxTree.h"
#include <stdlib.h>

// Function prototypes for the methods
static Node *getRootAST(AbstractSyntaxTree *);
static void addAST(Node *, AbstractSyntaxTree *);
static void removeAST(Node *, AbstractSyntaxTree *);
static void cleanUpTreeAST(AbstractSyntaxTree *);
static void reduceAST(AbstractSyntaxTree *);
static void betaReductionAST(Node *, bool *, AbstractSyntaxTree *);
static bool capturedVariableAST(Node *, const char *, AbstractSyntaxTree *);
static bool hasVariableAST(Node *, AbstractSyntaxTree *);
static void alfaConversionAST(Node *, const char *, const char *, AbstractSyntaxTree *);
static void replaceVarAST(Node *, const char *, Node *, AbstractSyntaxTree *);
static Node *copyTreeAST(Node *, Node *, AbstractSyntaxTree *);
static void printAST(Node *, const AbstractSyntaxTree *);

AbstractSyntaxTree *createAbstractSyntaxTree(ParseTree *parseTree) {
    AbstractSyntaxTree *obj = malloc(sizeof(AbstractSyntaxTree));
    if (obj == NULL) {
        longjmp(memoryAllocationException, 1);
        return NULL;
    }

    // Initialize fields
    obj->size = parseTree->getSize(parseTree);
    obj->root = parseTree->getRoot(parseTree);
    obj->nodes = parseTree->getNodes(parseTree);

    // Initialize function pointers
    obj->getRoot = getRootAST;
    obj->add = addAST;
    obj->remove = removeAST;
    obj->cleanUpTree = cleanUpTreeAST;
    obj->reduce = reduceAST;
    obj->betaReduction = betaReductionAST;
    obj->capturedVariable = capturedVariableAST;
    obj->hasVariable = hasVariableAST;
    obj->alfaConversion = alfaConversionAST;
    obj->replaceVar = replaceVarAST;
    obj->copyTree = copyTreeAST;
    obj->print = printAST;

    return obj;
}

void destroyAbstractSyntaxTree(AbstractSyntaxTree *obj) {
    if (obj == NULL) {
        return;  // Nothing to free
    }
    
    // Free resources associated with the nodes
    // You need to implement logic to properly free all nodes

    // Free the AbstractSyntaxTree object itself
    free(obj);
}

// Implementations of the methods (currently stubs)
static Node *getRootAST(AbstractSyntaxTree *this) {
    return this->root;
}

static void addAST(Node *node, AbstractSyntaxTree *this) {
    // not needed
}

static void removeAST(Node *node, AbstractSyntaxTree *this) {
    // not needed
}

static void cleanUpTreeAST(AbstractSyntaxTree *this) {
    for (int i = 1; i < this->size; i++) {
        Node * node = this->nodes[i];
        if (node->singleChild != NULL && strcmp(node->info.expression, "BACKSLASH") != 0 && strcmp(node->info.expression, "EMPTY") != 0) {
            node->singleChild->parent = node->parent;
            if (node->parent->leftChild == node){
                node->parent->leftChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
            else if (node->parent->rightChild == node) {
                node->parent->rightChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
            else if (node->parent->singleChild == node) {
                node->parent->singleChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
        } 
        if (node->info.expression == "EXPR1") {
            if (node->leftChild == NULL && node->rightChild == NULL) {
                node->parent->singleChild = node->parent->leftChild;
                node->parent->leftChild = NULL;
                node->parent->rightChild = NULL;
                node->info.expression = "EMPTY";
            }
        }
    }

    for (int i = 1; i < this->size; i++) {
        Node * node = this->nodes[i];
        if (node->singleChild != NULL && strcmp(node->info.expression, "BACKSLASH") != 0 && strcmp(node->info.expression, "EMPTY") != 0) {
            node->singleChild->parent = node->parent;
            if (node->parent->leftChild == node){
                node->parent->leftChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
            else if (node->parent->rightChild == node) {
                node->parent->rightChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
            else if (node->parent->singleChild == node) {
                node->parent->singleChild = node->singleChild;
                node->info.expression = "EMPTY";
            }
        } 
    }
    for (int i = 0; i < this->size; i++) {
        Node * node = this->nodes[i];
        if (strcmp(node->info.expression, "EXPR") == 0 || strcmp(node->info.expression, "EXPR1") == 0) {
            node->info.expression = "SPACE";
            node->info.string = " ";
        } 
    }
}

static void reduceAST(AbstractSyntaxTree *this) {
    // Implementation
}

static void betaReductionAST(Node *node, bool *reductionDone, AbstractSyntaxTree *this) {
    // Implementation
}

static bool capturedVariableAST(Node *node, const char *variable, AbstractSyntaxTree *this) {
    return false;
}

static bool hasVariableAST(Node *node, AbstractSyntaxTree *this) {
    return false;
}

static void alfaConversionAST(Node *node, const char *oldCharacter, const char *newCharacter, AbstractSyntaxTree *this) {
    // Implementation
}

static void replaceVarAST(Node *node, const char *variable, Node *substitution, AbstractSyntaxTree *this) {
    // Implementation
}

static Node *copyTreeAST(Node *root, Node *parent, AbstractSyntaxTree *this) {
    return NULL;
}

static void printAST(Node *node, const AbstractSyntaxTree *this) {
    // Implementation
    if (node == NULL) return;

    int parentIndex = (node->parent == NULL) ? -1 : node->parent->index;

    if (node->singleChild != NULL) {
        if (strcmp(node->info.expression, "BACKSLASH") == 0) {
            printf("(");
            printf("\\%s ", node->info.string);
            printAST(node->singleChild, this);
            printf(")");
        } else {
            printAST(node->singleChild, this);
            printf("%s", node->info.string);
        }
    } else {
        printf("(");
        printAST(node->leftChild, this);
        printf("%s", node->info.string);
        printAST(node->rightChild, this);
        printf(")");
    }
    if (node == this->root) printf("\n");
}

