#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
	int fd, uio_fd;
	char buf[32];
	char *uio_buf;
	fd = open("/dev/charDev0", O_RDWR);
	if(fd == -1)
		return -1;
	
	printf("open sucess\n");
	write(fd, "word", 5);
	read(fd, buf, 32);
	printf("read %s\n", buf);
	uio_fd = open("/dev/uio0", O_RDWR);
	void *pV = mmap(NULL, 0x400, PROT_READ | PROT_WRITE , MAP_SHARED, uio_fd, 0);
	if(pV == (void *)-1)
	{
		printf("mmap error\n");
		return -1;
	}
	close(uio_fd);
	uio_buf = (char *)pV;
	strcpy(uio_buf, "hello world");
	printf("%s\n",uio_buf);

	read(fd, buf, 32);
	printf("read %s\n", buf);

	munmap(pV, 0x40);
	close(fd);
	return 0;
}
