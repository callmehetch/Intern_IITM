
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	time_t starttime = time(NULL);
	int count = 0;

	while(time(NULL) - starttime < 10)
	{
		printf("\n\rNum = %d",count);
		count++;
	}
	
	return 0;
} 
	
