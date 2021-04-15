#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <FreeRTOS.h>

#include "tree.h"
#include "custom_queue.h"
#include "logger.h"

#define TASK_ACQUIRE_TR_LOCK() do { } while(0)
#define TASK_RELEASE_TR_LOCK() do { } while(0)

typedef LIST_HEAD(task_section_list_t, task_section_cons_t) task_section_list;
typedef void (*entry_ptr_t) (void *);
#define APPLICATION_TASK_PRIORITY 2


/* checkpoint defines */
typedef enum {
	cp_req_rtu,
	cp_req_tm
}cp_req_t;

typedef void (*request_hook_fn_t)(cp_req_t req_type);

/*
 * defines for task_register structure. Implemented as a rbtree for 
 * fast search when doing malloc()/free().
 */
typedef struct task_register_cons_t {
	const char		*name;
	Elf32_Ehdr		*elfh;
	xTaskHandle		 task_handle;
	request_hook_fn_t	 request_hook;
	void                    *cont_mem;
	size_t			 cont_mem_size;
	task_section_list	 sections;
	// task_dynmemsect_tree	 dynmemsects;
	// volatile migrator_lock	 migrator_lock;
	RB_ENTRY(task_register_cons_t) tasks;
} task_register_cons;

typedef RB_HEAD(task_register_tree_t, task_restister_cons_t)
    task_register_tree;

/* defines for task_rection structure. Implemented as double linked list */
typedef struct task_section_cons_t {
    const char *name;
    Elf32_Half section_index;
    void *amem;
    LIST_ENTRY(task_section_cons_t) sections;
}task_section_cons;

/*
 * Prototypes and compare function for task_register structure.
 */

static __inline__ int task_register_cons_cmp
(task_register_cons *op1, task_register_cons *op2)
{
	u_int32_t op1i = (u_int32_t)op1->task_handle;
	u_int32_t op2i = (u_int32_t)op2->task_handle;
	if (op1->task_handle == 0 || op2->task_handle == 0)
		return 1;
	else
		return (int64_t)op1i - (int64_t)op2i;
}
//RB_PROTOTYPE(task_register_tree_t, task_register_cons_t, tasks, task_register_cons_cmp)

int task_alloc(task_register_cons *trc);
task_register_cons *task_register(const char *name, Elf32_Ehdr *elfh);
int task_link(task_register_cons *trc);
int task_start(task_register_cons *trc);
Elf32_Sym *find_symbol(char *name, Elf32_Ehdr *elf_h);

#endif
