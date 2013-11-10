#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>

int main()
{
    char *aBuff;
    char pString[16]; 
    aBuff = malloc( 64 );
    /*... and write something into the memory */
    sprintf( aBuff, "Some text in the buffer" );
    /*Then convert the address of the buffer to a string*/
    sprintf( pString, "%p", aBuff );

    int fd;
    char * fifo = "/tmp/fifo";

    /* create the FIFO (named pipe) */
    mkfifo(fifo, 0666);

    /* write string to the FIFO */
    fd = open(fifo, O_WRONLY);
    write(fd, pString, sizeof(pString));
    close(fd);

    /* remove the FIFO */
    unlink(fifo);

    return 0;
}
