#include <stdio.h>
#include <stdlib.h>

FILE *ifp;
char *mode = "rt";	
char filename[] = "/dev/simp_malloc";
char line[64];
char *aBuff;
char *anotherBuff;
char *end;

int main(int argc, char *argv[]) {


	ifp = fopen(filename, mode);

	if (ifp == NULL) {
		fprintf(stderr, "Can't open input file!\n");
		return 1;
	}

	while(fgets(line, 64, ifp) != NULL) {
	 	/* get a line, up to 64 chars from fr.  done if NULL */
		sscanf (line, "%s");
  	}
   	fclose(ifp);  /* close the file prior to exiting the routine */

  	printf( "string representation of pointer: %s\n", line );
  	anotherBuff = (char*)strtol( line, &end, 16 );
  	printf( "anotherBuff=%p, and the contents of this memory is: \"%s\"\n", anotherBuff, anotherBuff );

}