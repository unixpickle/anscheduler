#include "threading.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
  void (* method)(void *);
  void * arg;
} newthread_args;

static uint64_t cpusLock = 0;
static cpu_info cpus[0x20];
static uint64_t cpuCount = 0;
__thread cpu_info cpu;

static void * thread_enter(newthread_args * args);

cpu_info * antest_get_current_cpu_info() {
  return &cpu;
}

void antest_launch_thread(void * arg, void (* method)(void *)) {
  pthread_t thread;
  newthread_args * args = malloc(sizeof(newthread_args));
  pthread_create(&thread, NULL, (void * (*)(void *))thread_enter, args);
}

void anscheduler_cpu_lock() {
  cpu_info * info = antest_get_current_cpu_info();
  assert(!info->isLocked);
  info->isLocked = true;
}

void anscheduler_cpu_unlock() {
  cpu_info * info = antest_get_current_cpu_info();
  assert(info->isLocked);
  info->isLocked = false;
  
  // TODO: here, check if a timer should be delivered
}

task_t * anscheduler_cpu_get_task() {
  return antest_get_current_cpu_info()->task;
}

thread_t * anscheduler_cpu_get_thread() {
  return antest_get_current_cpu_info()->thread;
}

void anscheduler_cpu_set_task(task_t * task) {
  antest_get_current_cpu_info()->task = task;
}

void anscheduler_cpu_set_thread(thread_t * thread) {
  antest_get_current_cpu_info()->thread = thread;
}

void anscheduler_cpu_notify_invlpg(task_t * task) {
  // nothing to do here since we don't actually do paging
}

void anscheduler_cpu_notify_dead(task_t * task) {
  // TODO: loop through here and set the next timer ticks of each CPU
}

void anscheduler_cpu_stack_run(void * arg, void (* fn)(void * a)) {
  // TODO: allocate CPU stacks
}

void anscheduler_cpu_halt() {
  assert(!antest_get_current_cpu_info()->isLocked);
  // TODO: here, sleep until timer tick time
}

static void * thread_enter(newthread_args * _args) {
  newthread_args args = *_args;
  free(_args);
  printf("in new thread!\n");
  args.method(args.arg);
  return;
}
