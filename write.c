/***************************************************************************
* Copyright (C) 2019 by Ismail Yesildirek & Bijan Kianian
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Ismail Yesildirek, Bijan Kianian, and the University of Colorado are not
* liable for any misuse of this material.
*
*****************************************************************************/
/**
* @file write.c
* @brief This source file contains a c program  that provides the write() function definition
* and writes to a prevously allocated block of memory a hex number.
* @author Ismail Yesildirek & Bijan Kianian
* @date February 24 2019
* @version 1.0
*
*/

#include "memtest.h"
#include "write.h"

void write(int* address, int offset, int value)
{
    *(address + offset) = value;
    printf(" Value: %x written at address: %p\n\n", value, address + offset);
	printf("Enter another command: \n");
    return;
}