# dynamic-scheduler

## Requirements:

1) Updating software without restarting the system.
2) Ensures high-availability.
3) Safely update or reconfigure a part of the system at a lower cost.
4) The granularity aimed here is udpating of independent software modules.
5) Efficient utilization of bandwidth, specifically when its limited in earth/space communication.

## Runtime Update Process:
1) Execution of task start.
2) Task executes for a while. (inconsistent state)
3) Task reaches check-pointable state i.e. consistent state where it doesn't have any external dependency or inflight transaction.
4) Run-time module updates the task.
5) Task state is transformed to checkpoint object.
6) Decheckpointer takes a checkpoint object and transforms into task state.
7) The task continues its execution again to reach an inconsistent state and to check-pointable state.

## Boot Process:
1) Boot time System binary (FreeRTOS) is loaded and execution start.
2) At run time system loads in and starts up the applications from application binaries.
3) Application binary linked to system and if needed other binaries.
4) Application binaries are compiled both position independent (can be loaded at any address) and relocatable (they can be linked to any binary and are updatable)

## Components Involved:
- Task Manager: 
  - layer upon FreeRTOS to handle run-time updatable ELF binaries I.e applications.
  - Provides framework for handling tasks built upon FreeRTOS
  - Manages data structure for each task 
- Task Register:
  - keeps track of all allocated memory sections I.e sections with allocated flags in ELF.
  - Keeps track of stack and dynamically allocated sections (via malloc). (task-register entry uses splay trees to keep track of memory allocation).
  - Every task register entry is an item in search tree(implemnted as Red-Black tree).
  - Key for search tree is the name of the task ( can be used ID in next version)
- Task Alloc:
  - Allocates sections described in the task ELF binary
