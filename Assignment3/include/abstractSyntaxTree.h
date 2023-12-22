/**
 * @file abstractSyntaxTree.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 */

// TODO: const correctness?

#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

#include "node.h"
#include "parseTree.h"
#include "bool.h"

typedef struct AbstractSyntaxTree AbstractSyntaxTree;

struct AbstractSyntaxTree {
    Node *root;
    Node **nodes;
    int size;

    // Function pointers for methods
    Node *(*getRoot)(AbstractSyntaxTree *);
    void (*add)(Node *, AbstractSyntaxTree *);
    void (*remove)(Node *, AbstractSyntaxTree *);
    void (*cleanUpTree)(AbstractSyntaxTree *);
    void (*reduce)(AbstractSyntaxTree *);
    bool (*possibleBetaReduction)(Node *, AbstractSyntaxTree *);
    void (*betaReduction)(Node *, bool *, AbstractSyntaxTree *);
    bool (*capturedVariable)(Node *, const char *, AbstractSyntaxTree *);
    bool (*hasVariable)(Node *, AbstractSyntaxTree *);
    void (*alfaConversion)(Node *, const char *, const char *, AbstractSyntaxTree *);
    void (*replaceVar)(Node *, const char *, Node *, AbstractSyntaxTree *);
    Node *(*copyTree)(Node *, Node *, AbstractSyntaxTree *);
    void (*print)(Node *, const AbstractSyntaxTree *);
};

// Function declarations
AbstractSyntaxTree *createAbstractSyntaxTree(ParseTree *);
void destroyAbstractSyntaxTree(AbstractSyntaxTree *);
// ... Other function declarations

#endif // ABSTRACT_SYNTAX_TREE_H

