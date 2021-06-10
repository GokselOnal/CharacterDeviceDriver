/*
    This is a Character Device Driver. This Device Driver have read and
    write functionality.
    Copyright (C) 2021  Göksel can ÖNAL, 
    			Nurettin Burak Altıntaş, 
    			Alperen Fatih Zengin.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/.
    GitHub link:  https://github.com/GokselOnal/CharacterDeviceDriver
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


#define DEVICE "/dev/project_char_device" 

int main(){
	int i,fd;
	char ch, write_buf[100], read_buf[100];

	fd = open(DEVICE, O_RDWR);

	if(fd == -1){
		printf("file is either does not exist or has been locked by another process\n");
		exit(-1);
	}
	while(1){
		printf("r = read from device,\nw = write to device command,\nq = quit,\nEnter command:");
		scanf("%c",&ch);
		/*clean the input buffer*/
		while ((getchar()) != '\n');

		switch(ch){
			case 'w':
				printf("enter data: ");
				scanf(" %[^\n]",(write_buf));
				/*clean the input buffer*/
				while ((getchar()) != '\n');
				write(fd, write_buf, sizeof(write_buf));
				break;
			case 'r':
						read(fd,read_buf, sizeof(write_buf));
						printf("device: %s\n",read_buf);
						break; 
			case 'q':
						close(fd);
						return 0;
			default:
				printf("command nor recongnized\n");
				break;
		}
	}
	
}
