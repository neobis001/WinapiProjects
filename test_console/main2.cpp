// crt_freopen_s.c  
// This program reassigns stderr to the file  
// named FREOPEN.OUT and writes a line to that file.  

#include <stdio.h>  
#include <stdlib.h>  
#include <iostream>

FILE *stream;

using namespace std;

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
		//fprintf(stderr, "This will go to the file 'freopen.out'\n");
		cerr << "Testing";
		fclose(stream);
	}
	system("type freopen.out");

	int a = (int)'1';
	cout << a << endl;
}