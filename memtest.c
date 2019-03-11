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
* @date March 10 2019
* @version 1.6
*
*/

#include "memtest.h"

int32_t memoryOffsetValue  = 0;       /* Offset value for the memory block to be allocated*/
int32_t* Block_Address = NULL;       /* Place holder for block address*/
int8_t allocStatus = 0;	/* Malloc Flag */

int main()
{
#if FRDM

	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

#endif

	printf("This is a simple program for testing memory.\r\n");
	printf("Type 'help' for more details or 'exit' to leave the program: \r\n");
    printf("PES_Prj1 >> ");
	int8_t validStatus = 0;

    while(validStatus == 0)
       {
          validStatus = inputCheck();
       }

    return 0;
}

/*########################################## inputCheck() [Start] #######################################################*/
int32_t inputCheck(void)
{
    char *cmds[] = { "help", "exit", "allocate", "free", "read", "write", "invert", "pattern", "verify"};         
	/* Constant strings to be compared with user input commands */
    char *Token[10] = {};               /* Array of strings for saving tokens in command line after parsing user input line*/
    char userInput[50] = {}, temp;      /* Array to store input command line string */
    uint32_t memoryValue;
    int32_t memoryAddress ;
    int32_t Block_Address_lo;		/* Seperating the lower 32 bits of block address
													to compare with user input address at Token[2] (immediate addressing)*/
    //int memoryAddressOffset;
    int32_t Seed;
    int32_t numberOfwords;					       /* Used in Pattern()*/
    int32_t startOffset;                         /* Memory location offset from start of the block */
    int32_t compareResult;
	char allocInput[6]; /*For re-alloc question*/
        /***** Parsing variables ******/
     char *pToken;                 /* Token pointer to be used in parsing command line input string*/
     int8_t tokenCount;               /* Counter used in parsing procedure*/
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
        int8_t value = 0;
        printf("Please enter a valid command, or <help> for details\n\n");
        printf("PES_Prj1 >> ");
        return value;
      }


/**************************************** Parsing Start *************************************************/
    tokenCount = 0;
    pToken = strtok (userInput," ");

	/*Check for an user input*/
	while (!pToken)
	{
		printf("Invalid Input. Please try again: \n");
		printf("PES_Prj1 >> ");
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

	int8_t valid = 0;
		if (strcmp(Token[0], cmds[0]) == 0)             /*     help()     */
			{
				valid = 0;
				help();
				printf("PES_Prj1 >> ");
			}
		else if (strcmp(Token[0], cmds[1]) == 0)      /*     exit()      */
			{
				valid = 1;
				printf("\nGood Bye!\n\n");
			}
		else if (strcmp(Token[0], cmds[2]) == 0)        /*   allocate()  */
			{
			   valid = 0;

		        if (Token[1] == 0 || atoi(Token[1]) == 0 || Token[2] != 0)             /* Error if no offset or more than 1 argument enterred*/
                    {
                        printf("Please enter not more than a valid non-zero offset value w/o any extra, or <help> for details\n");
                        printf("PES_Prj1 >> ");
                        return valid;
                    }

                if (allocStatus == 0)
                    {
                        memoryOffsetValue = atoi(Token[1]);
                        Block_Address = (int32_t*)allocate(memoryOffsetValue);
                        if (Block_Address)
                            {
                                printf("\nMemory block allocated.\n");
#if FRDM
                                printf("Address: %p    size: %ld   (%ld Bytes)\n", Block_Address, \
                                    memoryOffsetValue, sizeof(int32_t) * memoryOffsetValue);
#else
                                printf("Address: %p    size: %d   (%ld Bytes)\n", Block_Address, \
                                    memoryOffsetValue, sizeof(int32_t) * memoryOffsetValue);
#endif
                                allocStatus = 1;
                                /*One memory allocation performed*/
                                printf("Enter another command \n\n");
                                printf("PES_Prj1 >> ");
                            }
                    }
                else
                    {
                        printf("\nMemory is current allocated in address 0x%p\n", Block_Address);
                        printf("\nDo you wish to override this data? (Y?) \n");
                        printf("PES_Prj1 >> ");
                        fflush(stdin);
                        scanf("%[^\n]s", allocInput);	/* Accepting override Input*/
                        scanf("%c", &temp);		/* Flushing '\n' character from the stdin buffer */
                        //printf("\nYour answer is: %s \n", allocInput);
                            if (strcmp(allocInput, "yes") == 0 || strcmp(allocInput, "YES") == 0 || strcmp(allocInput, "y") == 0 || strcmp(allocInput, "Y") == 0)
                                {
                                    free_memory(Block_Address);
#if FRDM
                                    printf(" %ld Bytes of heap released from address %p to %p\n", \
                                    sizeof(int32_t) * memoryOffsetValue, Block_Address, Block_Address + memoryOffsetValue-1);
#else
                                    printf(" %ld Bytes of heap released from address %p to %p\n", \
                                    sizeof(int32_t) * memoryOffsetValue, Block_Address, Block_Address + memoryOffsetValue-1);
#endif

                                    allocStatus = 0;
                                    /*system ready to allocate memory again*/
                                    memoryOffsetValue = atoi(Token[1]);

                                    Block_Address = (int32_t*)allocate(memoryOffsetValue);
                                    if (Block_Address)
                                        {
                                            printf("\nMemory block allocated.\n");
#if FRDM
                                            printf("Address: %p    size: %ld   (%ld Bytes)\n", Block_Address, \
                                            memoryOffsetValue, sizeof(int32_t) * memoryOffsetValue);
#else
                                            printf("Address: %p    size: %d   (%ld Bytes)\n", Block_Address, \
                                            memoryOffsetValue, sizeof(int32_t) * memoryOffsetValue);	
#endif
                                            allocStatus = 1;
                                            /*One memory allocation performed*/
                                            printf("Enter another command \n\n");
                                            printf("PES_Prj1 >> ");
                                        }
                                }
                            else
                                {
                                    printf("\nMemory allocated remains in address 0x%p\n", Block_Address);
                                    printf("Enter another command \n\n");
                                    printf("PES_Prj1 >> ");
                                }

                    }
			}
		else if (strcmp(Token[0], cmds[3]) == 0)        /*    free()     */
			{
				valid = 0;

				if(!Block_Address)
                    {
                        printf("Memory is not allocated yet!\n");
                        printf("Enter another command \n\n");
                        printf("PES_Prj1 >> ");
                    }
				else
					{
						free_memory(Block_Address);
						printf(" %ld Bytes of heap released from address %p to %p\n\n", \
						sizeof(int32_t) * memoryOffsetValue,Block_Address, Block_Address + memoryOffsetValue-1);
                        Block_Address = NULL;
						allocStatus = 0;
						/*system ready to allocate memory again*/
						printf("Enter another command \n\n");
						printf("PES_Prj1 >> ");
					}
			}

		else if (strcmp(Token[0], cmds[4]) == 0)        /*    read()   */
		    {
               valid = 0;

                if(!(alloc_test(Token[0], Token[1], Token[2], Token[3], Token[4])))
					return valid;


				if(strcmp(Token[1] , "-i"))																					
					{																														
					/* For relative addressing:
					*  Token[1] != "-i" , Token[1] = <start offset>, Token[2] = <end offset >,Token[3] == 0. */
						startOffset = offsetCheck(Token[1]);
						numberOfwords = lengthCheck(Token[2], startOffset);

						if((startOffset == -1) || (numberOfwords == 0))
							return valid;

						display(Block_Address+startOffset, numberOfwords);
						return valid;
					}

		        if(!strcmp(Token[1] , "-i"))    																				
                    {																														
					/* For relative addressing:
					*  Token[1] != "-i" , Token[2] = <address>, Token[3] = <offset> */
						Block_Address_lo =  (LSB_WORD)Block_Address;
						memoryAddress = addressCheck(Token[2], Block_Address_lo);

				        if(memoryAddress == 0)
							return valid;
						startOffset = (memoryAddress - Block_Address_lo)/4;
						numberOfwords = lengthCheck(Token[3],startOffset);

						if(numberOfwords == 0)
							return valid;

						int32_t* Start = Block_Address + startOffset;
						display(Start, numberOfwords);
						return valid;
					}

			}

		else if (strcmp(Token[0], cmds[5]) == 0)        /*    write()     */
		    {
                valid = 0;

				if(!(alloc_test( Token[0], Token[1], Token[2], Token[3], Token[4])))
					return valid;

				if(strcmp(Token[1] , "-i"))																					
					{																														
					/* For relative addressing:
					*  Token[1] != "-i" , Token[1] = <offset>, Token[2] = <value>,Token[3] == 0. */
					
						startOffset = offsetCheck(Token[1]);
						memoryValue = valueCheck(Token[2]);

						if((startOffset == -1) || (memoryValue== -2))
							return valid;

						write(Block_Address, startOffset, memoryValue);
						return valid;
					}

                    if(!strcmp(Token[1] , "-i"))    /* immediate addressing : write < -i > < address(hex) > < value(hex) > */
                            {
                                Block_Address_lo =  (LSB_WORD)Block_Address;
                                memoryAddress= addressCheck(Token[2], Block_Address_lo);
								memoryValue = valueCheck(Token[3]);

								if((memoryAddress == 0) || (memoryValue == -2))
									return valid;
                                startOffset = 0;		/* No offset for immediate addresing */


								write(Block_Address + (memoryAddress - Block_Address_lo)/4, startOffset, memoryValue);
								return valid;
							}

            }
		else if (strcmp(Token[0], cmds[6]) == 0)        /*    invert()    */
            {
            valid = 0;
                if(!(alloc_test( Token[0], Token[1], Token[2], Token[3], Token[4])))
					return valid;

				if(strcmp(Token[1] , "-i")) /* Relative addressing <offset> < value>*/
                    {																														
					/*	Token[1] != "-i" , Token[1] = <offset>, Token[2] = <value>,Token[3] == 0. */
						startOffset = offsetCheck(Token[1]);
						numberOfwords = lengthCheck(Token[2], startOffset);

						if((startOffset == -1) || (numberOfwords == 0))
							return valid;

						invert_Time( startOffset, numberOfwords);
						return valid;
					}


				  if(!strcmp(Token[1] , "-i"))    /* immediate addressing : write < -i > < address(hex) > < value(hex) > */
					{
						Block_Address_lo =  (LSB_WORD)Block_Address;
						memoryAddress = addressCheck(Token[2], Block_Address_lo);

						if(memoryAddress == 0)
							return valid;

						startOffset = (memoryAddress - Block_Address_lo)/4;
						numberOfwords = lengthCheck(Token[3], startOffset);

						if(numberOfwords == 0)
							return valid;


						invert_Time( startOffset, numberOfwords);
						return valid;
                    }
            }
        else if (strcmp(Token[0], cmds[7]) == 0)        /*    pattern()    */
            {
				valid = 0;
                if(!(alloc_test( Token[0], Token[1], Token[2], Token[3], Token[4])))
					return valid;

				if(strcmp(Token[1] , "-i")) /* Relative addressing <offset> < value>*/
                    {
						startOffset = offsetCheck(Token[1]);
						numberOfwords = lengthCheck(Token[2], startOffset);

						if((startOffset == -1) || (numberOfwords == 0))
							return valid;

						Seed = seedCheck(Token[3]);
						if(Seed == -1)
							return valid;

						pattern_Time( startOffset, numberOfwords,Seed );
					}


				  if(!strcmp(Token[1] , "-i"))    /* immediate addressing : write < -i > < address(hex) > < value(hex) > */
					{

						Block_Address_lo =  (LSB_WORD)Block_Address;
						memoryAddress = addressCheck(Token[2], Block_Address_lo);

						if(memoryAddress == 0)
							return valid;

						startOffset = (memoryAddress - Block_Address_lo)/4;
						numberOfwords = lengthCheck(Token[3], startOffset );

						if(numberOfwords == 0)
							return valid;


						Seed = seedCheck(Token[4]);
						if(Seed == -1)
							return valid;

						pattern_Time( startOffset,  numberOfwords, Seed );
					}
            }
        else if (strcmp(Token[0], cmds[8]) == 0)        /*    verify()    */
            {
				valid = 0;
                if(!(alloc_test(Token[0], Token[1], Token[2], Token[3], Token[4])))
					return valid;

				if(strcmp(Token[1] , "-i")) /* Relative addressing <offset> < value>*/
                    {
						startOffset = offsetCheck(Token[1]);
						numberOfwords = lengthCheck(Token[2], startOffset);

						if((startOffset == -1) || (numberOfwords == 0))
							return valid;

						Seed = seedCheck(Token[3]);
						if(Seed == -1)
							return valid;

						verify( Block_Address,startOffset, numberOfwords,Seed );
					}


				  if(!strcmp(Token[1] , "-i"))    /* immediate addressing : write < -i > < address(hex) > < value(hex) > */
					{

						Block_Address_lo =  (LSB_WORD)Block_Address;
						memoryAddress = addressCheck(Token[2], Block_Address_lo);

						if(memoryAddress == 0)
							return valid;

						startOffset = (memoryAddress - Block_Address_lo)/4;
						numberOfwords = lengthCheck(Token[3], startOffset );

						if(numberOfwords == 0)
							return valid;


						Seed = seedCheck(Token[4]);
						if(Seed == -1)
							return valid;

						verify( Block_Address, startOffset,  numberOfwords, Seed );
					}
            }
		else
		    {
		        valid = 0;
                printf("Invalid input, please try again: \n");
                printf("PES_Prj1 >> ");
		    }

	return valid;
}
 /*###################################### inputCheck () [End] ###############################################*/

 /**                               			    { Functions Decfinision }                                					  **/

 /************************************** pattern_Time() [Start]*********************************************/

	void pattern_Time(int32_t StartOffset, int32_t NumberOfwords,int32_t Seed)
	{
               /**
                * Invert Byte Block per wordSize selected
                * and start timer.
                **/
#if FRDM

	 uint32_t count = 0;			/* Counter value */
	 uint32_t elapsedTime = 0;		/* time value in micro or nano seconds */
	 uint16_t done = SysTick_Config(16000000);		/* Enable the counting, every 1 microsec the counter 'count' will increment */
	 if(done)
	   printf("Reset Systick");

	 #else
                clock_t executionT;
                executionT = clock();
#endif
                pattern(Block_Address + StartOffset, NumberOfwords, Seed);
#if FRDM
	count = SYSTICK_MAX - SysTick->VAL;		/* SysTick->VAL holds the decremented value of systick timer register */
											/* 'count' holds number of ticks, each tick takes ~21 nanosecond (48 MHZ clock)*/
	SysTick->CTRL = 4 ;						/* Disable couting = 0b100 */
	if ((21*count) >=100000)
		{
			elapsedTime = 21*count / 1000;	/* For demonstrating 3 digit in micro seconds */
			printf("Pattern function execution time is %ld microseconds\n", elapsedTime);
		}
	else
		{
			elapsedTime = 21*count;
			printf("Pattern function execution time is %ld nanoseconds\n", elapsedTime);
		}
#else
                executionT = clock() - executionT;
                double time = ((double)executionT)/CLOCKS_PER_SEC;
                printf("Pattern function execution time is milli: %fsec.\n",time*1000);
#endif
                printf("Enter another command \n\n");
                printf("PES_Prj1 >> ");
				return;
	}
/************************************** pattern_Time() [End]************************************************/

/************************************** offsetCheck() [Start] ***************************************/

int32_t offsetCheck(char* str)
{
	int32_t Offset = atoi (str);        /* Starting memory location offset from Block_Address*/

					/* Condtion check for valid offset between 0 to block size (memoryOffsetValue) */

						if((Offset < 0 ) || (Offset > (memoryOffsetValue-1)))
							{
#if FRDM
								printf("Please enter valid offset between 0 to %ld\n", memoryOffsetValue-1);
#else
                                printf("Please enter valid offset between 0 to %d\n", memoryOffsetValue-1);
#endif
								printf("PES_Prj1 >> ");
								return -1;
							}
		return Offset;
}
/******************************************offsetCheck() [End] *************************/

/******************************************lengthCheck() [Start] ************************/
int32_t lengthCheck(char* str, int32_t startOffset)
{
	int32_t length = atoi (str);       /* Number of locations (words) to display */
		if((length > (memoryOffsetValue - startOffset)) || length <= 0)
							{
#if FRDM															
								printf("Please enter valid length between 1 to %ld\n", \
								memoryOffsetValue-startOffset);
#else
								printf("Please enter valid length between 1 to %d\n", \
								memoryOffsetValue-startOffset);
#endif
								printf("PES_Prj1 >> ");
								return 0;
							}
	return length;
}
/*****************************************lengthCheck() [End] ***************************/

/***************************************** valueCheck() [Start] ************************/

int32_t valueCheck(char* str)
{

	 int32_t validInput = strcspn(str, "ghijklmnopqrstuvwxyz.GHIJKLMNOPQRSTUVWXYZ,][{}`+-*/"); /* Validating correct hex number*/

                                if(validInput < strlen(str) ||(validInput > 8))
                                    {
                                        printf("Please enter a valid 32bit hex number for the value\n");
                                        printf("PES_Prj1 >> ");
                                        return -2;
                                    }

                                 uint32_t memoryValue = strtoll(str, NULL, 16); /* Converting string to hex */

		 return memoryValue;
 }
/****************************************valueCheck() [End] **************************/


/************************************** addressCheck() [Start] ************************************/

int32_t addressCheck(char* str1, int32_t Block_Address_lo)
{

	int32_t memoryAddress;                /* Starting address for immediate addressing < -i >  in write() and read()*/
    int32_t validInput;

    validInput = strcspn(str1, "ghijklmnopqrstuvwxyz.GHIJKLMNOPQRSTUVWXYZ,][{}`+-*/"); /* Validating start address for correct hex number*/

						if((validInput < strlen(str1) || (validInput != ADDRESS_LENGTH)))	/* Maximum 12 characters for address value on PC */
							{
								printf("Please enter a valid hex number for the memory address\n");
								printf("PES_Prj1 >> ");
								return 0;
							}

						memoryAddress = strtol(str1, NULL, 16); /* Converting input address string to hex,*/

						/* Validating the starting address being in the range of allocated block */

						if ((memoryAddress- Block_Address_lo > 4*(memoryOffsetValue-1)) ||  /* 4 bytes distance between two immediate memory addresses*/
     						(memoryAddress < Block_Address_lo))
							{
								printf("Please enter the starting memory address between %p and %p\n", Block_Address, Block_Address + memoryOffsetValue-1);
								printf("PES_Prj1 >> ");
								return 0;
							}
						if(((memoryAddress- Block_Address_lo) % 4 ) != 0)
							{
								printf("Invalid address, did you mean: %p or %p ?\n", \
								Block_Address + (memoryAddress- Block_Address_lo)/4 , \
								Block_Address + ((memoryAddress- Block_Address_lo)/4 + 1)); /* To make sure the address is in 4 byte increment-
																							-between two immediate memory addresses*/
								printf("PES_Prj1 >> ");
								return 0;
							}

        return memoryAddress;
}
/**************************************addressCheck [End] ************************************/

/************************************** alloc_test() [Start] *************************************************/
//	This function tests at the beginnign of each user command for aloocated memory block and proper arguments.
//        if it is not allocated, or address/offset or value are not provided, an error message will inform the user

int32_t alloc_test(char* str0, char* str1, char* str2, char* str3, char* str4)
{

				if(!Block_Address)
                    {
                        printf("Memory is not allocated yet!\n\n");
                        printf("PES_Prj1 >> ");
                        return 0;
                    }

                  if ((str1 == 0 || str2 == 0) || (!strcmp(str1,"-i") && str3 == 0) \
						|| (((!strcmp(str0, "pattern") || (!strcmp(str0,"verify")))) && str3 == 0) \
						|| (((!strcmp(str0, "pattern") || (!strcmp(str0,"verify")))) && (!strcmp(str1,"-i") && str4 == 0)) )  /* No offset/value enterred*/
                    {
                        printf("Please enter sufficient parameters for this command, or <help> for details\n");
                        printf("PES_Prj1 >> ");
                        return 0;
                    }

			return 1;
}
/************************************** alloc_test() [End] *************************************************/

/**************************************invert_Time() [Start]************************************************/
 void invert_Time (int32_t startOffset, int32_t numberOfwords )
 {
	 int32_t wordSize = numberOfwords;
	/**
	* Invert Byte Block per wordSize selected
	* and start timer.
	**/
#if FRDM

	 uint32_t count = 0;			/* Counter value */
	 uint32_t elapsedTime = 0;		/* time value in micro or nano seconds */
	 uint16_t done = SysTick_Config(16000000);		/* Enable the counting, every 1 microsec the counter 'count' will increment */
	 if(done)
	   printf("Reset Systick");

#else
	clock_t executionT;				/* For PC platform under Linux */
	executionT = clock();

#endif
	while ((numberOfwords <= wordSize) && (numberOfwords -1>=0))
		{
			invert(Block_Address+startOffset-1, numberOfwords);
			numberOfwords--;
		}
#if FRDM
	count = SYSTICK_MAX - SysTick->VAL;		/* SysTick->VAL holds the decremented value of systick timer register */
											/* 'count' holds number of ticks, each tick takes ~21 nanosecond (48 MHZ clock)*/
	SysTick->CTRL = 4 ;						/* Disable couting = 0b100 */

	if ((21*count) >=100000)
		{
			elapsedTime = 21*count / 1000;	/* For demonstrating 3 digit in micro seconds */
			printf("Invert function execution time is %ld microseconds\n", elapsedTime);
		}
	else
		{
			elapsedTime = 21*count;
			printf("Invert function execution time is %ld nanoseconds\n", elapsedTime);
		}

#else
	executionT = clock() - executionT;
	double time = ((double)executionT)/CLOCKS_PER_SEC;
	printf("Invert function execution time is milli: %f sec.\n", time*1000);

#endif
	printf("Enter another command \n\n");
	printf("PES_Prj1 >> ");
 }

 /***************************************** invert_Time() [End]*************************************/

 /**************************************** seedCheck() [Start] *************************************/

 int32_t seedCheck(char *str)
 {
	  if (str != 0)
		{
			int32_t validInput = strcspn(str,\
			 "abcdefghijklmnopqrstuvwxyz.ABCDEFGHIJKLMNOPQRSTUVWXYZ,][{}`/+-*"); /* Validating correct int32_t number for Seed*/

			if(validInput < strlen(str) ||(validInput > 10))/* Check if the number(seed) in larger than 10 digits
																	                 (as in max 31 bit interger = 2,147,483,647)*/
				{
					printf("Please enter a positive decimal number for the 'Seed' between 0 and 2,147,483,647\n");
					printf("PES_Prj1 >> ");
					return -1;
				}
		}
			int32_t Seed = atoi (str);        /* Converting string to number */

	return Seed;
 }
/**************************************** seedCheck() [End] *************************************/
