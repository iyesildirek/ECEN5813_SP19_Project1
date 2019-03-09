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
/*
* @file verify.c
* @brief This source file contains code that provides the verify() function definition
* which is used to verify the pattern().
*
* @author Ismail Yesildirek & Bijan Kianian
* @date March 6 2019
* @version 1.01
*/

#include "memtest.h"
#include "verify.h"

void verify(int* Block_Address, int StartOffset, int NumberOfwords, unsigned int userSeed)
{
	unsigned int a = 1664525;    /* Multiplier */
    unsigned int c = 1013904223; /* Increment */
    unsigned long int m = pow(2,32);       /* Modulus */
	int Seed = userSeed;
	clock_t executionT;
	int identical =0;

	pattern(Block_Address + StartOffset, NumberOfwords, userSeed);
	executionT = clock();
	for (int i = 0; i < NumberOfwords-1; i++)
		{
			Seed = (a * Seed + c) % m;
			int value = *(Block_Address + StartOffset + i);
			if (value != Seed)
			{
				printf("Discrepancy at address: %p\n", Block_Address+StartOffset + i);
				printf("Expected value: %x, Actual value: %x\n", Seed, *(Block_Address + StartOffset + i));
				identical = 1;
			}
		}
		  executionT = clock() - executionT;
          double time = ((double)executionT)/CLOCKS_PER_SEC;
		if (identical == 0)
			printf("The expected and actual values are identical!\n");

		printf("Pattern function execution time is milli: %fsec.\n",time*1000);
		printf("Enter another command \n\n");
		printf("PES_Prj1 >> ");

		return;
}


