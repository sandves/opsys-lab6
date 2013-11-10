#include <stdio.h>
#include <stdlib.h>

FILE *ifp;
char *mode = "r";	
char filename[] = "/dev/simp_malloc";
char line[64];
char *aBuff;
char *anotherBuff;
char *end;

int main(int argc, char *argv[]) {


	ifp = fopen(filename, mode);

	if (ifp == NULL) {
		fprintf(stderr, "Unable to open %s. Verify that the file exists.\n", filename);
		return 1;
	}

	while(fgets(line, 64, ifp) != NULL) {
		sscanf (line, "%s");
  	}
   	fclose(ifp);

  	printf( "string representation of pointer: %s\n", line );
  	anotherBuff = (char*)strtol( line, &end, 16 );
  	printf( "anotherBuff=%p, and the contents of this memory is: \"%s\"\n", anotherBuff, anotherBuff );

}