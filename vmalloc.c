/*
Project Name: Memory Leakage Detector
Author :Swanand Sapre
*/

//Include Files
#include	<stdio.h>
#include	<stdint.h>
#include	<signal.h>
#include "vmalloc.h"

//Constants


#define SUCCESS					(0)
#define ERROR_UNDEFINED			(-1)
#define ERROR_ADDRESS_NULL		(-2)
#define ERROR_LIST_EMPTY		(-3)
#define ERROR_NODE_NOT_FOUND	(-4)



 


//Structure to keep track of allocated memory
typedef struct mem_rec_block{
	void *alloc_address;
	size_t bytes;
	struct mem_rec_block *next;
	struct mem_rec_block *prev;
}mem_rec_t;

void *alloc_address=NULL;
mem_rec_t *new_block;
mem_rec_t *head=NULL;
mem_rec_t *tail=NULL;

//Handler which Prints the Leakage Information on SIGINT Singnal
void sigintHandler(int sig_num){
	print_leaks();
	exit(sig_num);	
}
//Record the newly assigned address into mem_rec_block
int add_to_mem_rec(void *allocated_heap_addr, size_t requested_byes){
	
	int error_code=SUCCESS;
	
	if(allocated_heap_addr==NULL || requested_byes <=0){
		error_code=ERROR_ADDRESS_NULL;
		return error_code;
	}
	
	new_block = (mem_rec_t *) malloc(sizeof(mem_rec_t));
	new_block->alloc_address=(void *)allocated_heap_addr;
	new_block->bytes=requested_byes;
	new_block->next=NULL;
	new_block->prev=NULL;
	
	//handle first Entry	
	if(head==NULL){
	  	head=new_block;
	  	tail=new_block;
	  	//printf("add_to_list head: 0x%X \n", head->alloc_address); 
  	}
	
	else{	
		new_block->prev=tail;	
		tail->next=new_block;
		tail=new_block;
		//printf("add_to_list: tail 0x%X \n", tail->alloc_address); 
	}
 	//print_leaks(); 	
	return  error_code;
}

int delete_from_mem_rec(void *mem_free){
	int error_code=SUCCESS;
	mem_rec_t *curr=NULL, *delete_node=NULL;
	//printf("delete_from_mem_rec 0x%X \n",mem_free);
	
	if(mem_free==NULL){
		error_code=ERROR_ADDRESS_NULL;
		return error_code;
	}
	
	if(head==NULL){
		error_code=ERROR_LIST_EMPTY;
		return error_code;
	}
	
	//Delete Head Node
	if(head!=NULL && head->alloc_address==mem_free){

		delete_node=head;
		head=head->next;
		if(head){
			head->prev=NULL;
		}		
		free(delete_node);	
		//printf("delete_from_mem_rec head: 0x%X \n",mem_free);
		return error_code;
	}
	curr=head;
	while(curr!=NULL){	
		if(curr->alloc_address == mem_free){
			//printf("delete_from_mem_rec curr: 0x%X \n",curr->alloc_address);
		     delete_node=curr;
			curr->prev->next=curr->next;
			if(curr->next){
			curr->next->prev=curr->prev;		
			}			
			curr=curr->next;
			free(delete_node);
			return error_code;
		}
		curr=curr->next;		
	}
	if(curr==NULL){
		error_code=ERROR_NODE_NOT_FOUND;
	}
	//printd("delete_from_mem_rec","Exit");
	//print_leaks();
	return error_code;
}

//This is a wrapper Function to malloc
void* vmalloc(size_t requested_bytes){

	if(requested_bytes<=0){
		return NULL;
	}
	
	alloc_address=(void *)malloc(requested_bytes);
	if(alloc_address==NULL){
		return NULL;
	}

	int error_code=add_to_mem_rec(alloc_address,requested_bytes);
	if(error_code>=0){
		return alloc_address;
	}
	else{
		return NULL;
	}
}

//This is a wrapper Function to free
int vfree(void* mem_free){
	int error_code=delete_from_mem_rec(mem_free);
	if(error_code>=0){
		free(mem_free);
		//print_leaks();
	}
	return error_code;

}

//Print mem_rec_List
void print_leaks(){
	printf("\n-----------Memory Leakage Detector-----------------\n");
	mem_rec_t *curr;
	curr=head;
	if(curr){
	printf("\nBelow are the Memory Leaks:\n");
	}
	else{
	printf("No Memory Leaks in the Program.\n");
	}
	while(curr!=NULL){
	printf("Adress: 0X%X Size: %d Byte \n ",curr->alloc_address,curr->bytes);
	curr=curr->next;
	}
	printf("\n------------------------------------------------------\n");

}
