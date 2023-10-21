/**
 * @file parseTree.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 */

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <node.h>

typedef struct {
    
} ParseTree;

// constructor 
Lexer * createParseTree(TokenString * tokenString);
// destructor
void destroyParseTree(Lexer * obj);

#endif // PARSE_TREE_H
