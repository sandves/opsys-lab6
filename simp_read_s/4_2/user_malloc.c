/*****************************************************************************
DESCRIPTION A small main() that opens shows how to convert a pointer
to a string and convert it back to a pointer
*****************************************************************************/
/*--------------------  I n c l u d e   F i l e s  -------------------------*/
#include<stdio.h> 
#include<stdlib.h>
/*--------------------  C o n s t a n t s  ---------------------------------*/
/*--------------------  T y p e s  -----------------------------------------*/
/*--------------------  V a r i a b l e s  ---------------------------------*/
/*--------------------  F u n c t i o n s  ---------------------------------*/
int main() {
  char *aBuff;
  char *anotherBuff;
  char pString[16]; 
  char *end;
  /*Allocate some memory*/
  aBuff = malloc( 64 );
  /*... and write something into the memory */
  sprintf( aBuff, "Some text in the buffer" );
  /*Then convert the address of the buffer to a string*/
  sprintf( pString, "%p", aBuff );
  /*We now have a string (pString) where we have stored a pointer */ 
  /*This string can be printed and read as text */ 
  printf( "aBuff=%s\n", pString );
  /* Now onvert the string back to a ’real’ pointer*/
  anotherBuff = (char*)strtol( pString, &end, 16 );
  /* The easy way would have been to do like this: */
  /* anotherBuff = aBuff */
  /* but instead we have gone through convertion to text */
  /* finaly print what our new pointer is pointing to*/
  /* which should be the same as aBuff is pointing to*/ 
  printf( "anotherBuff=%p, and the contents of this memory is: \"%s\"\n", anotherBuff, anotherBuff );
  /* release out allocated memory to avoid memory lekage */
  free( aBuff );
  return 0; 
}