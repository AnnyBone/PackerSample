#include "shared.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("Value test: %s\nAnorther value: %s\n", value_test.buffer,value_another.buffer);
	
	printf("Press ENTER to close");
	getchar();

    return 0;
}

