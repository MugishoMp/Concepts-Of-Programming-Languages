/**
 * @file fileReader.h
 * @author Mugisho Mpozi s2440547
 * @brief 
 * @version 0.1
 * @date 2023-10-20
 * 
 */

#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdbool.h>

/**
 * @brief Reads input from a file.
 *
 * This function reads lines from a file specified by @p readFileName and stores
 * them in the character array pointed to by @p expression.
 *
 * @param expression Pointer to a character array where the read input will be 
 * stored. This pointer will be allocated dynamically.
 * @param readFileName Name of the file to read from.
 *
 * @return Returns true if the file was successfully opened and read, false 
 * otherwise.
 */
bool readInput(char **expression, const char *readFileName);

#endif // FILE_READER_H
