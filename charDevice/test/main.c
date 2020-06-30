#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd;
	char buf[32];
	fd = open("/dev/chardev", O_RDWR);
	if(fd == -1)
		return -1;
	
	write(fd, "word", 5);
	read(fd, buf, 32);
	printf("read %s\n", buf);
	close(fd);
	return 0;
}
