
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "task.h"
#include "scheduler_algorithm.h"

#define SCHEDULER_NAME "PRR"

const struct scheduler_operations sops;

int verbose_flag = 1;

static int handle;

static struct task 	*h1 = NULL, 
					*h2 = NULL, 
					*h3 = NULL, 
					*h4 = NULL;

int init_module(void) {
	handle = register_scheduler(SCHEDULER_NAME, &sops);
	if(handle < 0){
		eprintf("Failed registering scheduler PRR with error %d\n", handle);
		return handle;
	}
	else{
		dprintf("Registered scheduler PRR\n");
	}
	return 0;
}

void cleanup_module(void) {

	int rv = unregister_scheduler(handle);
	if(rv < 0) eprintf("Error in unregister_scheduler: %d\n", rv);
	
}

struct task* prr_enqueue(struct task* r, struct task* t) {

	struct task** h;
	if(t->task_info->priority == 0)
		h = &h1;
	else if(t->task_info->priority == 1)
		h = &h2;
	else if(t->task_info->priority == 2)
		h = &h3;
	else
		h = &h4;

	if(*h == NULL){
	
		*h = t;
		t->next = *h;
		t->prev = *h;
		
	}else{
	
		(*h)->prev->next = t;
		t->prev = (*h)->prev;
		(*h)->prev = t;
		t->next = (*h);
		
	}
	
	return *h;
}

struct task* prr_dequeue(struct task* r) {

	struct task** h;
	
	if(r->task_info->priority == 0){
		h = &h1;
	}
	else if(r->task_info->priority == 1){
		h = &h2;
	}
	else if(r->task_info->priority == 2){
		h = &h3;
	}
	else{
		h = &h4;
	}

	if(r->next == r){
		*h = NULL;
	}
	else {
	
		r->prev->next = r->next;
		r->next->prev = r->prev;
		*h = r->next;
		
	}
	
	return *h;
}

struct task* prr_periodic_timer(struct task* r) {

	if(h1){
		h1 = h1->next;
		return h1;
	}
	
	else if(h2){
		h2 = h2->next;
		return h2;
	}
	
	else if(h3){
		h3 = h3->next;
		return h3;
	}
	
	else if(h4){
		h4 = h4->next;
		return h4;
	}
	
	return NULL;
}

const struct scheduler_operations sops = {
	.task_enqueue = prr_enqueue,
	.task_dequeue = prr_dequeue,
	.periodic_timer = prr_periodic_timer,
	.period = 1
};
