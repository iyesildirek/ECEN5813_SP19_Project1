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
* @file memtest.c
* @brief This source file contains a c program to manipulate and test memory.
*
* @author Ismail Yesildirek & Bijan Kianian
* @date February 24 2019
* @version 1.4
*
*/

#include "memtest.h"

int memoryOffsetValue  = 0;        /* Offset value for the memory block to be allocated*/
int* Block_Address = NULL;            /* Place holder for block address*/
int allocStatus = 0;	/* Malloc Flag */ 

int main()
{
	printf("This is a simple program for testing memory.\n");
	printf("Type 'help' for more details or 'exit' to leave the program: \n");

	int validStatus = 0;

    while(validStatus == 0)
       {
          validStatus = inputCheck();
       }

    return 0;
}
/*########################################## inputCheck() Start #######################################################*/
int inputCheck(void)
{
    char *cmds[] = { "help", "exit", "allocate", "free", "read", "write", "invert"};         /* Constant strings to be compared with user input commands */
    char *Token[10] = {};               /* Array of strings for saving tokens in command line after parsing user input line*/
    char userInput[50] = {}, temp;      /* Array to store input command line string */
    int memoryValue;                    /* Value to be written at memory locaion defined by write()*/
    int location =0;                       /* Memory location offset from start of the block */
    int compareResult;
	char allocInput[6]; /*For re-alloc question*/
        /***** Parsing variables ******/
     char *pToken;                 /* Token pointer to be used in parsing command line input string*/
     int tokenCount;               /* Counter used in parsing procedure*/
     char **str[10];               /* Array of strings pointed to by the token*/
        /*******************************/

     fflush(stdin);                 /* Flushing keyboard buffer from previous input*/
     strcpy(userInput," ");         /* Reseting userInput string array*/
	 strcpy(allocInput," ");         /* Reseting allocInput string array*/
     scanf("%[^\n]", userInput);	/* Accepting user Input*/
     scanf("%c", &temp);            /* Flushing '\n' character from the stdin buffer after user hit the 'Enter' */
     compareResult = strcmp(userInput,"");
     while(compareResult == 32)      /* Validating user input, No input = 'SPACE' (i.e decimal value 32)*/
      {
        int value = 0;
        printf("Please enter a valid command, or <help> for details\n\n");
        return value;
      }


/**************************************** Parsing Start *************************************************/
    tokenCount = 0;
    pToken = strtok (userInput," ");

	/*Check for an user input*/
	while (!pToken)
	{
		printf("Invalid Input. Please try again: \n");
		fflush(stdin);
		scanf("%[^\n]s", userInput);	/* Accepting user Input*/
		scanf("%c", &temp);		/* Flushing '\n' character from the stdin buffer */
		pToken = strtok(userInput, " ");
	}

    str[0] = &pToken;
	     while (pToken)
	       {
             Token[tokenCount] = *str[tokenCount];    /* Saving the parsed texts into *str[] array*/
             pToken = strtok (NULL, " ");

             tokenCount++;
             str[tokenCount] = &pToken;
	       }
/****************************************** Parsing End **************************************************/

	int valid = 0;
		if (strcmp(Token[0], cmds[0]) == 0)             /*     help()     */
			{
				valid = 0;
				help();
			}
		else if (strcmp(Token[0], cmds[1]) == 0)      /*     exit()      */
			{
				valid = 1;
				printf("\nGood Bye!\n\n");
			}

		else if (strcmp(Token[0], cmds[2]) == 0)        /*     allocate()    */
			{
			   valid = 0;

		       if (Token[1]==0)             /* No offset enterred*/
                    {
                         printf("Please enter a valid offset value, or <help> for details\n");
                         return valid;
                    }
				else
				{
			if (allocStatus == 0)
			{
				memoryOffsetValue = atoi(Token[1]);
				//printf("String to int value = %d", memoryOffsetValue);
				Block_Address = allocate(memoryOffsetValue);
				if (Block_Address)
				{
					printf("\nMemory block allocated.\n");
					printf("Address: %p    size: %d   (%ld Bytes)\n", Block_Address, \
						memoryOffsetValue, sizeof(int) * memoryOffsetValue);
					allocStatus = 1;
					/*One memory allocation performed*/
					printf("Enter another command: \n");
				}
			}
			else
			{
				printf("\nMemory is current allocated in address 0x%p\n", Block_Address);
				printf("\nDo you wish to override this data? (Y?) \n");
				fflush(stdin);
				scanf("%[^\n]s", allocInput);	/* Accepting override Input*/
				scanf("%c", &temp);		/* Flushing '\n' character from the stdin buffer */
				//printf("\nYour answer is: %s \n", allocInput);
				if (strcmp(allocInput, "yes") == 0 || strcmp(allocInput, "YES") == 0 || strcmp(allocInput, "y") == 0 || strcmp(allocInput, "Y") == 0)
				{
					free_memory(Block_Address);
					printf(" %ld Bytes of heap released from address %p to %p\n", \
					sizeof(int) * memoryOffsetValue, Block_Address, Block_Address + memoryOffsetValue);
					allocStatus = 0;
					/*system ready to allocate memory again*/
					memoryOffsetValue = atoi(Token[1]);
					//printf("String to int value = %d", memoryOffsetValue);
					Block_Address = allocate(memoryOffsetValue);
					if (Block_Address)
					{
						printf("\nMemory block allocated.\n");
						printf("Address: %p    size: %d   (%ld Bytes)\n", Block_Address, \
						memoryOffsetValue, sizeof(int) * memoryOffsetValue);
						allocStatus = 1;
						/*One memory allocation performed*/
						printf("Enter another command: \n");
					}
				}
				else 
					{
					printf("\nMemory allocated remains in address 0x%p\n", Block_Address);
					printf("Enter another command: \n");
					}
					
				}

			}
			}
		else if (strcmp(Token[0], cmds[3]) == 0)        /*    free()    */
			{
				valid = 0;

				if(!Block_Address)
				{
					printf("Memory is not allocated yet!\n");
					printf("Enter another command: \n");
				}
				else
					{
						free_memory(Block_Address);
						printf(" %ld Bytes of heap released from address %p to %p\n\n", \
					           sizeof(int) * memoryOffsetValue,Block_Address, Block_Address + memoryOffsetValue-1);
                        Block_Address = NULL;
						allocStatus = 0;
						/*system ready to allocate memory again*/
						printf("Enter another command: \n");
					}
			}


		else if (strcmp(Token[0], cmds[4]) == 0)        /*    display()    */
		    {
                valid = 0;

                if(!Block_Address)
                    {
                        printf("Memory is not allocated yet!\n\n");
                        return valid;
                    }

                else  if (Token[1] == 0 || Token[2] == 0)     /* No offset/value enterred*/
                    {
                         printf("Please enter a valid starting offset and number of words, or <help> for details\n");
                         return valid;
                    }

                int startOffset = atoi (Token[1]);        /* Starting memory location offset from Block_Address*/

        /* Condtion check for valid offset between 0 to block size (memoryOffsetValue) */

                if( (startOffset < 0 ) || (startOffset > (memoryOffsetValue-1)))
                    {
                        printf("Please enter valid offset between 0 to %d\n", memoryOffsetValue-1);
                        return valid;
                    }


                int numberOfwords = atoi ( Token[2]);       /* Number of locations (words) to display */

                if(numberOfwords > (memoryOffsetValue - startOffset))
                    {
                        printf("Please enter valid number of words between 1 to %d\n", \
                        memoryOffsetValue-startOffset);
                        return valid;
                    }

                display(Block_Address+startOffset, numberOfwords);

            }

		else if (strcmp(Token[0], cmds[5]) == 0)        /*    write()    */
		    {
                valid = 0;

                if(!Block_Address)
                    printf("Memory is not allocated yet!\n\n");

                /* Condition to check if user entered correct number of arguments*/

                else  if (Token[1] == 0 || Token[2] == 0)     /* No offset/value enterred*/
                    {
                         printf("Please enter a valid memory location and value, or <help> for details\n");
                         return valid;
                    }

                else if (Token[2] != 0)
                    {
                         int validInput = strcspn(Token[2], "ghijklmnopqrstuvwxyz"); /* Validating correct hex number*/

                         if(validInput < strlen(Token[2]) ||(validInput > 8))
                            {
                                printf("Please enter a valid 32bit hex number for the memory value\n");
                                return valid;
                            }

                        memoryValue = strtol(Token[2], NULL, 16); /* Converting string to hex */
                        location = atoi (Token[1]);         /* *(address + location) = place to write the value*/

                        /* A condition to check the correct offset value between 0 and maximum offset derived by allocate() */

                        if ((location > memoryOffsetValue-1) || (location < 0))
                            {
                                printf("Please enter a memory location between 0 and %d\n", memoryOffsetValue-1);
                                return valid;
                            }

                        write(Block_Address, location, memoryValue);

                    }

            }
		else if (strcmp(Token[0], cmds[6]) == 0)        /*    invert()    */
		{
		valid = 0;
		if (!Block_Address)
		{
		printf("Memory is not allocated yet!\n\n");
		return valid;
		}

		else  if (Token[1] == 0 || Token[2] == 0)     /* No offset/value enterred*/
		{
		printf("Please enter a valid starting offset and number of words, or <help> for details\n");
		return valid;
		}

		int startOffset = atoi(Token[1]);        /* Starting memory location offset from Block_Address*/

/* Condtion check for valid offset between 0 to block size (memoryOffsetValue) */

		if ((startOffset < 0) || (startOffset > (memoryOffsetValue - 1)))
		{
			printf("Please enter valid offset between 0 to %d\n", memoryOffsetValue - 1);
			return valid;
		}

		int numberOfwords = atoi(Token[2]);       
		/* Number of locations (words) to display */
		if (numberOfwords > (memoryOffsetValue - startOffset))
		{
			printf("Please enter valid number of words between 1 to %d\n", \
				memoryOffsetValue - startOffset);
			return valid;
		}
		int wordSize = numberOfwords;
		/**
		* Invert Byte Block per wordSize selected
		* and start timer.
		**/
		clock_t executionT; 
		executionT = clock();
		while ((numberOfwords <= wordSize) && (numberOfwords >=0))
		{
		invert(Block_Address+startOffset, numberOfwords);
		numberOfwords--;
		}
		executionT = clock() - executionT;
		double time = ((double)executionT)/CLOCKS_PER_SEC;
		printf("Invert function execution time is milli: %fsec.\n",time*1000);
		printf("Enter another command: \n");
		}
			
		else
		    {
		        valid = 0;
                printf("Invalid input, please try again: \n");
		    }

	return valid;
}
/*###################################### inputCheck () End #############################################*/

