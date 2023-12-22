/**
 * @file parseTree.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 */

// TODO: const correctness?

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <node.h>

typedef struct {
    Node * root;
    Node ** nodes;
    int size;

    int (*getSize)(void*);
    void (*setSize)(int, void*);
    Node * (*getRoot)(void*);
    void (*setRoot)(Node *, void*);
    Node ** (*getNodes)(void*);
    void (*setNodes)(Node **, void*);

    void (*addNode)(Node *, void*);
    void (*removeNode)(Node *, void*);
    void (*printParseTree)(void*);
    void (*printDisambiguatedExpression)(void*);
    

} ParseTree;

// constructor 
ParseTree * createParseTree();
// destructor
void destroyParseTree(ParseTree * obj);

#endif // PARSE_TREE_H
