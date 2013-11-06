#include <stdio.h>
#include <stdlib.h>

FILE *ifp, *ofp;
char *mode = "rt";	
char filename[] = "/dev/simp_malloc";
char line[64];
char pString[16];
char *aBuff;
char *anotherBuff;
char *end;

int main(int argc, char *argv[]) {

	/*ifp = fopen(filename, mode);

	if (ifp == NULL) {
		fprintf(stderr, "Can't open input file!\n");
		return 1;
	}

	while(fgets(line, 64, ifp) != NULL) {
	 	/* get a line, up to 64 chars from fr.  done if NULL */
	//	sscanf (line, "%s");
  	//}
   	//fclose(ifp);  /* close the file prior to exiting the routine */


  	//anotherBuff = (char*)strtol( line, &end, 16 );
  	//printf( "anotherBuff=%p, and the contents of this memory is: \"%s\"\n", anotherBuff, anotherBuff );

	ofp = fopen(filename, "w");

	if (ofp == NULL) {
		fprintf(stderr, "Can't open output file!\n");
		return 1;
	}

	aBuff = malloc( 64 );
  	/*... and write something into the memory */
  	sprintf( aBuff, "Some text in the buffer" );
  	/*Then convert the address of the buffer to a string*/
  	sprintf( pString, "%p", aBuff );

	fwrite( pString , sizeof(char) , sizeof(pString) , ofp );

	fclose(ofp);  /* close the file prior to exiting the routine */

	return 0;
}
