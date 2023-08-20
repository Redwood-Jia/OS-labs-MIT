#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child_proc(int fd[2])
{
	//child processes
	//open(p[0]);//the second process have to read first
	close(fd[1]);
	int buf1,buf2;
	if (read(fd[0],&buf1,4) != 4) {
		printf("Error!the second process can't read from pipe!");
		exit(1);
	}
	printf("prime: %d\n",buf1);
	//when to end the recursive?
	if (read(fd[0],&buf2,4) != 0) {
		int newfd[2];
		pipe(newfd);

		//same as main()
		if (fork() == 0) {
			child_proc(newfd);
		} else {
			close(newfd[0]);//read from fd[0]
			//read a new number from left just now,but we haven't compare it yet
			if (buf2 % buf1 != 0) 
				write(newfd[1], &buf2, 4);

			//then we should read other numbers in pipes,until it was empty
			while(read(fd[0],&buf2,4) != 0) 
				if(buf2 % buf1 !=0)
					write(newfd[1], &buf2, 4);
			//after finished
			close(fd[0]);
			close(newfd[1]);
			wait(0);
			exit(0);
		}
	}
		//after child process complete it's work,close the write of pipe
		//close(p[1]);
	exit(0);
	return;
}

int main(int argc, char *argv[])
{
	//file descriptor
    int fd[2];
	pipe(fd);

	if(fork() == 0) {
		child_proc(fd);
		//don't use stacks...
		// while(fork() == 0) {
		// 	int buf2; 
		// 	if (buf2 % buf1 != 0) {
		// 		//maybe it's a prime
		// 	} else {
		// 	}
		// }
	} else { //parent program
		//write to pipe
		//close file descriptors we don't need at present 
		close(fd[0]);
		//then write
		int validWriteT = 0;
		int num = 1;//4 bytes,begins from 2
		while(validWriteT < 34) {
			num++;
			int tmp;
			tmp = write(fd[1],&num,4);
			if(tmp != 4){
				printf("%d Error!Parent process can't write it to the pipe",num);
				exit(1);
			} 
			validWriteT++;
		}
		close(fd[1]);
		//wait for child processes to be done
		wait(0);
		exit(0);
	}

	//in case
	printf("the end of the programme");
	exit(1);
}