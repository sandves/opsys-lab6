#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
 
char *string;
char ptr[16];
 
void *thread_function(void*);
 
int main(int argv, char *argc[]) {
  string = malloc(8);
  strcpy(string, "Heisann");
  sprintf(ptr, "%p", string);
 
  pthread_t thread;
 
  pthread_create(&thread, NULL, thread_function, NULL);
 
  strcpy(string, "hallllo");
  printf("[master] I have the string '%s' stored on '%p' in my userspace.\n",
         string, string);
 
  pthread_join(thread, NULL);
}
 
void *thread_function(void* _) {
  sleep(1);
  printf("[thread] I'm trying to read the string stored in '%s', and that is: '%s'\n",
         ptr, (char*)strtol(ptr, &string, 16));
}
