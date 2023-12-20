/**
 * @file expressionChecker.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

#ifndef EXPRESSION_CHECKER_H
#define EXPRESSION_CHECKER_H

int isWhitespace(char c);

int isEmptyExpression(char *expression);

/**
 * @brief reads the expression string
 *
 * This function reads the expression string and performs lexical analysis on
 * the expression.
 *
 * @param expression Pointer to a character array where the expression is stored
 * dynamically. This pointer will be allocated dynamically.
 *
 * @return Returns true if the expression is valid, false 
 * otherwise.
 */
void checkExpression(char **expression);



#endif // EXPRESSION_CHECKER_H
