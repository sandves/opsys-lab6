#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    char * fifo = "/tmp/fifo";
    char pString[16];
    char *anotherBuff;
    char *end;

    /* open, read, and display the string from the FIFO */
    fd = open(fifo, O_RDONLY);
    read(fd, pString, 16);
    anotherBuff = (char*)strtol( pString, &end, 16 ); 
  	printf( "anotherBuff=%p, and the contents of this memory is: \"%s\"\n", anotherBuff, anotherBuff );
    close(fd);

    return 0;
}
