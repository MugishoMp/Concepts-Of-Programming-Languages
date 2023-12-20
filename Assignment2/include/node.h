
/**
 * @file node.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 */


#ifndef NODE
#define NODE

typedef struct {
    char *expression;
    char *string;
} ExpressionStringPair;

/**
 * @brief
 * The node object that we insert in the Parse Tree
 * and Abstract Syntax Tree.
 *
 */
// Forward declaration of Node for self-referential structures
typedef struct Node Node;

struct Node {
    ExpressionStringPair info;
    Node *parent;
    Node *leftChild;
    Node *rightChild;
    Node *singleChild;

    int index;
};

#endif
