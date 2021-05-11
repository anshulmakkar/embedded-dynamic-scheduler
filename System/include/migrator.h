#ifndef _MIGRATOR_H_
#define _MIGRATOR_H_

#include "task_manager.h"
#include "elf.h"
#include "logger.h"
#include "applications.h"

int migrator_runtime_update(task_register_cons *trc, Elf32_Ehdr *new_sw);

int migrator_task_loop();

void migrator_task();

int migrator_task_start();

#endif
