#ifndef _COMMON_H_
#define _COMMON_H_

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>

// syscalls
#define gettime(ts_ptr)                   syscall(333, ts_ptr)
#define print_proc(pid, st_ptr, ft_ptr)   syscall(334, pid, st_ptr, ft_ptr)

// unit time
#define unit_time()  { volatile unsigned long i; for (i=0;i<1000000UL;i++); }

// scheduling policies
enum { FIFO, RR, SJF, PSJF };

// CPU assignment
#define SCHED_CPU   0   /* The CPU that the scheduler assigned to */
#define PROC_CPU    2   /* The CPU that processes assigned to */
void assign_cpu(int pid, int cpu);

// Priority
enum { LOW, HIGH };
void set_priority(int pid, int level);

// Processes
struct process {
    char name[32];
    int ready_time;
    int exec_time;
    int pid;
};
void fork_process(struct process *proc);

// Scheduler
#define TQ 500  /* Time quantum for RR */
void run_processes(int policy, int n_procs, struct process *procs);

#endif /* _COMMON_H_ */

