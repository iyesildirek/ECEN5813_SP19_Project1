  //***********     help()    *****************
#include "memtest.h"

  void help (void)
  {
    printf(" mem_test commands:\n");
    printf("\thelp - this message\n");
    printf("\tallocate <word> - allocate <words> of memory\n");
    printf("\tfree - free previously allocated memory\n");
    printf("\tread <address> [<words>] - read from <address> for one or <words>\n");
    printf("\twrite <address> <value>] - write <value> to <address>\n");
    printf("\tinvert <address> <words>] -invert bits starting from <address> for <words>\n");
    printf("\tpattern <address> <words> [<seed>] - write pseudorandom pattern at <address> for <words> using <seed>\n");
    printf("\tverify <address> <words> [<seed>] - verify pseudorandom pattern at <address> for <words> using <seed>\n");
    printf("\texit - exit the program\n");
    
    return;
  }
  
    
