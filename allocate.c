/*****************************************************************************
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
* @file allocate.c
* @brief This source file contains code that provides the allocate() function definition
* and frees the space allocated in the memory pointed by ptr.
*
* @author Ismail Yesildirek & Bijan Kianian
* @date February 23 2019
* @version 1.0
*
*/

#include "memtest.h"

int* allocate(int offset)
{

  int *ptr = (int*)calloc(offset, sizeof(int));  // assigning an int pointer to the block of memory with requested size use for calloc
//	int *ptr = (int*)malloc((sizeof(int))*offset);  // assigning an int pointer to the block of memory times the requested size use for malloc
	/*Malloc returns void ptr with # of Bytes*/

	if (ptr == NULL)                          // check if there is a room available in the heap
	{
		printf("Allocation failed!\n");
	}
	return ptr;
}

