#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
        // write(2,"Usage example:sleep 10", strlen("Usage example:sleep 10"))
		fprintf(2, "Usage example: sleep 10\n");
        //wrong
		exit(1);
	}

    //thansfer argv[1] from a sting into an integer
	int sleepTime = atoi(argv[1]);
	sleep(sleepTime);
	
	exit(0);
}