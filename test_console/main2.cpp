// crt_freopen_s.c  
// This program reassigns stderr to the file  
// named FREOPEN.OUT and writes a line to that file.  

#include <stdio.h>  
#include <stdlib.h>  

FILE *stream;

int main(void)
{
	errno_t err;
	// Reassign "stderr" to "freopen.out":   
	err = freopen_s(&stream, "freopen.out", "w", stderr);

	if (err != 0)
		fprintf(stdout, "error on freopen\n");
	else
	{
		fprintf(stdout, "successfully reassigned\n"); fflush(stdout);
		fprintf(stream, "This will go to the file 'freopen.out'\n");
		fclose(stream);
	}
	system("type freopen.out");
}