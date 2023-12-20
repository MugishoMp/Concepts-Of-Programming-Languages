/**
 * @file parseTree.c
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseTree.h"
#include "errorHandling.h"
#include <tokenString.h>




int getSize(void* self) {
    return ((ParseTree*)self)->size;
}

void setSize(int size, void* self) {
    ((ParseTree*)self)->size = size;
}

Node * getRoot(void* self) {
    return ((ParseTree*)self)->root;
}

void setRoot(Node * root, void* self) {
    ((ParseTree*)self)->root = root;
}

Node ** getNodes(void* self) {
    return ((ParseTree*)self)->nodes;

}

void setNodes(Node ** nodes, void* self) {
    ((ParseTree*)self)->nodes = nodes;
}

void addNode(Node * node, void* self) {
    ParseTree * this = ((ParseTree*)self);
    node->index = this->size;

    if ((int) this->size == 0) {
        this->root = node;
        this->nodes = malloc(sizeof(Node *));
        this->nodes[0] = node;
        this->size++;
        return;
    }

    Node **new_nodes = realloc(this->nodes, (this->size + 1) * sizeof(Node *));
    if (new_nodes == NULL) {
        return;  // Memory allocation failed
    }
    this->nodes = new_nodes;
    this->nodes[this->size] = node;

    if (strcmp(node->parent->info.expression, "EXPR") == 0) {
        if (strcmp(node->info.expression, "LEXPR") == 0) 
            node->parent->leftChild = node;
        else if (strcmp(node->info.expression, "EXPR1") == 0) 
            node->parent->rightChild = node;
    } else if (strcmp(node->parent->info.expression, "EXPR1") == 0) {
        if (strcmp(node->info.expression, "LEXPR") == 0) 
            node->parent->leftChild = node;
        else if (strcmp(node->info.expression, "EXPR1") == 0) 
            node->parent->rightChild = node;
    } else if (strcmp(node->parent->info.expression, "LEXPR") == 0) {
        if (strcmp(node->info.expression, "PEXPR") == 0) 
            node->parent->singleChild = node;
        else if (strcmp(node->info.expression, "BACKSLASH") == 0) 
            node->parent->singleChild = node;
    } else if (strcmp(node->parent->info.expression, "PEXPR") == 0) {
        if (strcmp(node->info.expression, "VARIABLE") == 0)
            node->parent->singleChild = node;
        else if (strcmp(node->info.expression, "EXPR") == 0) 
            node->parent->singleChild = node;
    } else if (strcmp(node->parent->info.expression, "BACKSLASH") == 0) {
            node->parent->singleChild = node;
    } 
    this->size++;
}

void removeNode(Node * node, void* self) {
    // removes the node and its subtree?
}


void printString(Node * root) {
    // printf("test-1\n");
    if (root == NULL) {
        // printf("test0\n");
        return;
    }

    // printf("test1\n");
    // printf("test1: %d\n", root->index);
    // printf("test1.1\n");
    if (root->singleChild != NULL){
        // printf("test2a\n");
        // printf("%s", root->info.string);
        printString(root->singleChild);
        // printf("test3\n");
    } else {
        // printf("test2b\n");
        if (root->leftChild != NULL) printString(root->leftChild);
        // printf("%s", root->info.string);
        // printf("test2c\n");
        if (root->rightChild != NULL) printString(root->rightChild);
    }

}

void printParseTree(void* self) {
    ParseTree * this = ((ParseTree*)self);
    if ((int) this->size == 0) {
        printf("tree is empty");
        return;
    }
    
    printf("test0");
    for (int i = 0; i < this->size; i++) {
        printf("parsetree index %d: %s\n", i, this->nodes[i]->info.expression);
    }

    printString(this->getRoot(this));


}



ParseTree * createParseTree(void) {
    ParseTree * obj = malloc(sizeof(ParseTree));
    if (obj == NULL) {
        longjmp(memoryAllocationException, 1);
        return NULL;
    }

    // fields
        obj->size = 0;
        obj->root = NULL;
        obj->nodes = NULL;

    // functions
        obj->getSize = getSize;
        obj->setSize = setSize;
        obj->getRoot = getRoot;
        obj->setRoot = setRoot;
        obj->getNodes = getNodes;
        obj->setNodes = setNodes;
        obj->addNode = addNode;
        obj->removeNode = removeNode;
        obj->printParseTree = printParseTree;

    return obj;
}

void destroyParseTree(ParseTree *obj) {
    if (obj == NULL) {
        return;  // Nothing to free
    }
    // delete all of the nodes in the parse tree as well

    // Free the TokenString object itself
    free(obj);
}
