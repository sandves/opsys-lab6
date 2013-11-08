#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
 
char *buffer;
char *threadBuffer;
char *end;
char pString[16];
 
void *dereference_pointer(void*);
 
int main(int argv, char *argc[]) {
  buffer = malloc(64);
  sprintf( buffer, "Some text in the buffer" );
  sprintf( pString, "%p", buffer );
  printf("[main] Address = %p, data = %s\n",
         buffer, buffer);
 
  pthread_t thread;
  pthread_create(&thread, NULL, dereference_pointer, NULL);
  pthread_join(thread, NULL);

  return 0;
}
 
void *dereference_pointer(void* _) {
  sleep(1);
  threadBuffer = (char*)strtol( pString, &end, 16 );
  printf("[pthread] Address = %s, data = %s\n",
         pString, threadBuffer);
}
