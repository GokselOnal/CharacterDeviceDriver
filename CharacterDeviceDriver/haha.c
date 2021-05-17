#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/vcs"

int main(char* args){
	int i,fd;
	char ch, write_buf[100], read_buf[100];

	fd = open(args, O_RDWR);

	if(fd == -1){
		printf("file %s either does not exist or has been locked by another process\n", args);
		exit(-1);
	}
	printf("r = read from device,\nw = write to device command: ");
	scanf("%c",&ch);

	switch(ch){
		case 'w':
			printf("enter data: ");
			scanf(" %[^\n]",(write_buf));
			write(fd, write_buf, sizeof(write_buf));
			break;
		case 'r':
                	read(fd,read_buf, sizeof(write_buf));
                	printf("device: %s\n",read_buf);
                	break; 
		default:
			printf("command nor recongnized\n");
			break;
	}

	close(fd);

	return 0;
}
