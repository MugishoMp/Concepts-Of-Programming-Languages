 /**
 * @file errorHandling.h
 * @author Mugisho Mpozi s2440547
 * @brief Emulate try catch blocks by saving the jump address here
 * @version 0.1
 * @date 2023-10-20
 * 
 */

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <setjmp.h>

/**
 * @brief The extern keyword in C is used to declare a variable that is defined 
 * in another file. When you declare a variable with extern, you're telling the 
 * compiler that the variable exists, but you're not allocating storage for it. 
 * The actual definition of the variable should be present in another file, and 
 * storage will be allocated there. This allows multiple files to use the same 
 * global variable or function declaration.
 * 
 */

extern jmp_buf env;
extern jmp_buf env2;

#endif // ERROR_HANDLING_H
