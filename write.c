<<<<<<< HEAD
/***************************************************************************
=======
/*****************************************************************************
>>>>>>> 42ef48c76e84d82839600c721cfadf26700d407b
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
<<<<<<< HEAD
* @brief This source file contains a c program to manipulate and test memory.
=======
* @brief This source file contains code that provides the write() function definition 
* and modifies the memory at the indicated location.
>>>>>>> 42ef48c76e84d82839600c721cfadf26700d407b
*
* @author Ismail Yesildirek & Bijan Kianian
* @date February 23 2019
* @version 1.0
*
*/

#include "memtest.h"

<<<<<<< HEAD
void write(int* address, int offset, int value)
{
    *(address + offset) = value;
    printf(" Value: %x written at address: %p\n\n", value, address + offset);
    return;
}
=======


>>>>>>> 42ef48c76e84d82839600c721cfadf26700d407b
