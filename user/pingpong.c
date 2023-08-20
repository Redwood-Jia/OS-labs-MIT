#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
	//the byte to be conveyed
	char toPass = 'a';
	char* toPassptr = &toPass;

	//use p[] to save file descriptor
		int p[2];
	pipe(p);

	int pid;
	pid = fork();

	if(pid > 0)
	{//parent process
		//write
		if(write(p[1],toPassptr,1) != 1){
			printf("Can't write to child!\n");
			exit(1);
		}
		close(p[1]);//then release the write privilege(?)
		wait((int *) 0);//waiting for child to execute;wait((int *) pid)??
		//receive
		char toReceive;
		char *toReceiveptr = &toReceive;
		if(read(p[0],toReceiveptr,1) != 1){
			//printf("Can't read from child!\n");
			exit(1);
		}
		close(p[0]);
		printf("%d:received pong\n",getpid());
		exit(0);
	}
	else 
	{//child process
		//receive
		char toReceive;
		char *toReceiveptr = &toReceive;
		if(read(p[0],toReceiveptr,1) != 1){
			printf("Can't read from parent!\n");
			exit(1);
		}
		close(p[0]);
		//print what it has received
		printf("%d:received ping\n",getpid());

		//write
		if(write(p[1],toPassptr,1) != 1){
			printf("Can't write to parent!\n");
			exit(1);
		}
		close(p[1]);
		exit(0);
	}
	//remenber to exit from MY program
	exit(0);
}