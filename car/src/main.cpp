#include <stdio.h>
#include "main.h"
#include "test/test.h"

int main(void)
{
	printf("car!!\n");
	printf("version:%d.%d\n",VERSION_MAJOR,VERSION_MINOR);
	test();
	return 0;
}
