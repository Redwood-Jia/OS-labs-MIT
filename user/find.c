#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void to_find(char *path,char *target) 
{
	//same as user/ls.c
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	//open the directory
	if((fd = open(path, 0)) < 0){
	fprintf(2, "find: cannot open %s\n", path);
	return;
	}

	//fatst(int fd,struct stat *st):place an info about an open file into *st
	if(fstat(fd, &st) < 0){
	fprintf(2, "find: cannot stat %s\n", path);
	close(fd);
	return;
	}

	//recursive the files under the path to find target
	//here,st.type should be a dir
	switch(st.type){
		case T_FILE:
			fprintf(2,"find usage:find dir filename\n");
			break;
			
		case T_DIR:
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
			printf("ls: path too long\n");
			break;
			}
			//name of path
			strcpy(buf, path);
			p = buf+strlen(buf);
			*p++ = '/';

			while(read(fd, &de, sizeof(de)) == sizeof(de)){
				//if the dir has no files
				if(de.inum == 0 || strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0)
					continue;
				
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;//end of the string
				//path is in buf
				if(stat(buf, &st) < 0){
					printf("ls: cannot stat %s\n", buf);
					continue;
				}

				if (st.type == T_DIR) {//sub directories
					to_find(buf,target);
				}
				else if (st.type == T_FILE) {
					if(strcmp(de.name,target) == 0) //find the target
						printf("%s\n",buf);
				}
				else 
					;
			}
			break;
  	}
	close(fd);
	return;
}

int main(int argc, char *argv[])
{
	//deal with input mistakes
	if (argc != 3)
	{
		fprintf(2, "find usage: find dir filename\n");
		exit(1);
	}
	//record path and file
	char *path = argv[1];
	char *target = argv[2];
	
	to_find(path, target);
	exit(0);
}