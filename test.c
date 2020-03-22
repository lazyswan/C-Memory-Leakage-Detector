/*
Project Name: Memory Leakage Detector
Author :Swanand Sapre
*/

#include	<stdio.h>
#include	<stdint.h>
#include 	<signal.h>
#include "vmalloc.h"

int main(){
	signal(SIGINT,sigintHandler);
	#pragma exit print_leaks

	void __attribute__((destructor)) print_leaks();
	
	
	printf("Hello World\n");
	int *iptr =(int*)vmalloc(10*sizeof(int));
	float *fptr =(float*)vmalloc(20*sizeof(float));
	char *cptr =(char*)vmalloc(30*sizeof(char));
		
	vfree(iptr);
	//vfree(fptr);
	vfree(cptr);
	
	printf("Good Bye World \n");
	while(1);	
return 0;
}
