#ifndef __ANSCHEDULER_TASK_H__
#define __ANSCHEDULER_TASK_H__

#include "types.h"

#define ANSCHEDULER_TASK_KILL_REASON_SELF 0
#define ANSCHEDULER_TASK_KILL_REASON_EXTERNAL 1
#define ANSCHEDULER_TASK_KILL_REASON_MEMORY 2

// maximum of 0x100000 threads when it's done this way
#define ANSCHEDULER_TASK_CODE_PAGE              0x400
#define ANSCHEDULER_TASK_KERN_STACKS_PAGE    0x100000
#define ANSCHEDULER_TASK_USER_STACKS_PAGE    0x200000
#define ANSCHEDULER_TASK_DATA_PAGE         0x10200000

task_t * anscheduler_task_create();

/**
 * Adds a task's to the scheduling queue.
 * @param task A reference is not needed here since the task is presumed not
 * to be part of the scheduling queue yet.
 * @critical
 */
void anscheduler_task_launch(task_t * task);

/**
 * Requests that a task be removed from the scheduling queue and notifies all
 * CPUs to stop running it.
 * @param task This task must have a reference to it.
 * @critical
 */
void anscheduler_task_kill(task_t * task, uint64_t reason);

/**
 * While references are held to a task, it cannot be killed.
 * @return false if the task has been killed.
 * @critical
 */
bool anscheduler_task_reference(task_t * task);

/**
 * See task_reference(). Note that if the reference count gets to 0 and the
 * task has been killed, a kill system thread will be generated for the task
 * in this function.
 * @critical
 */
void anscheduler_task_dereference(task_t * task);

/**
 * Finds a launched task with a specified PID. The returned task is
 * referenced, so you must dereference it yourself.
 * @critical With many tasks, this may take some time, despite the fact that
 * a PID hashmap is used.
 */
task_t * anscheduler_task_for_pid(uint64_t pid);

/**
 * Exit the current task with a specified code. This will automatically
 * switch to the CPU stack for you and run the next task in the loop. This
 * will not return.
 * @critical
 */
void anscheduler_task_exit(uint8_t code);

#endif
