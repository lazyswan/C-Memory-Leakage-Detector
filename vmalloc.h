/*
Project Name: Memory Leakage Detector
Author :Swanand Sapre
*/


#ifndef _V_MALLOC_
#define _V_MALLOC_
void sigintHandler(int sig_num);
void* vmalloc(size_t requested_bytes);
int vfree(void*);
void print_leaks();
#endif
