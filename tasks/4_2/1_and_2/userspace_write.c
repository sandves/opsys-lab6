#include <stdio.h>
#include <stdlib.h>

FILE *ofp;
char *mode = "w";	
char filename[] = "/dev/simp_malloc";
char pString[16];
char *aBuff;

int main(int argc, char *argv[]) {

	ofp = fopen(filename, mode);

	if (ofp == NULL) {
		fprintf(stderr, "Unable to open %s. Verify that the file exists.\n", filename);
		return 1;
	}

	aBuff = malloc( 64 );
  	/*... and write something into the memory */
  	sprintf( aBuff, "Some text in the buffer" );
  	/*Then convert the address of the buffer to a string*/
  	sprintf( pString, "%p", aBuff );
  	printf("string representation of pointer: %s\n", pString);

	fwrite( pString , sizeof(char) , sizeof(pString) , ofp );

	fclose(ofp);  /* close the file prior to exiting the routine */

	return 0;
}
