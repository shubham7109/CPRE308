
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "task.h"
#include "scheduler_algorithm.h"

#define SCHEDULER_NAME "SRTN"

static struct task *head = NULL;
const struct scheduler_operations sops;
int verbose_flag = 1;
static int handle;


typedef struct storage struct_obj;
struct storage {
	int reach;
	int ref;
};


int init_module(void) {
	handle = register_scheduler(SCHEDULER_NAME, &sops);
	if(handle < 0){
		eprintf("Failed registering scheduler SRTN with error %d\n", handle);
		return handle;
	}
	else{
		dprintf("Registered scheduler SRTN\n");
	}
	return 0;
}

void cleanup_module(void){
	int rv = unregister_scheduler(handle);
	if(rv < 0) eprintf("Error in unregister_scheduler: %d\n", rv);
}

struct task* srtn_enqueue(struct task* r, struct task* t) {

	struct_obj* mt;

	if(t->scheduler_data == NULL){
	
		mt = calloc(1, sizeof(struct_obj));
		mt->reach = 2*(4 - t->task_info->priority);
		mt->ref = 0;
		t->scheduler_data = mt;
		
	}
	else{
	
		mt = (struct_obj*) t->scheduler_data;
		mt->ref = t->task_info->run_time - mt->ref;
		mt->reach = 0.2*mt->reach + 0.8*mt->ref;
		t->scheduler_data = mt;
		
	}
	

	if(head == NULL){
	
		head = t;
		t->next = head;
		t->prev	= head;
		
	}
	
	else{
	
		struct_obj* mt1;
		mt = (struct_obj*) t->scheduler_data;
		mt1 = (struct_obj*) r->scheduler_data;
		
		while(mt1->reach < mt->reach){
			if(r == head)
				break;
			r = r->next;
			mt1 = (struct_obj*) r->scheduler_data;
		}

		r->prev->next = t;
		t->prev	= r->prev;
		r->prev = t;
		t->next	= r;
		head = r;
		
	}

	return head;
}

struct task* srtn_dequeue(struct task* r) {
	if(r->next == r){
		head = NULL;
	}
	
	else{
	
		r->prev->next = r->next;
		r->next->prev = r->prev;
		head = r->next;
		
	}
	
	return head;
}

const struct scheduler_operations sops = {
	.task_enqueue = srtn_enqueue,
	.task_dequeue = srtn_dequeue,
};

