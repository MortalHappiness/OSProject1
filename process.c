#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ========================================

void assign_cpu(int pid, int cpu)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &mask) != 0) {
        perror("sched_setaffinity");
        exit(1);
    }
}

void set_priority(int pid, int level)
{
    int policy;
    struct sched_param param;
    param.sched_priority = 0;

    switch (level) {
        case LOW:
            policy = SCHED_IDLE;
            break;
        case HIGH:
            policy = SCHED_OTHER;
            break;
        default:
            fprintf(stderr, "Invalid priority level: %d\n", level);
            break;
    }
    if (sched_setscheduler(pid, policy, &param) < 0) {
        perror("sched_setscheduler");
        exit(1);
    }
}

void fork_process(struct process *proc)
{
    int pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {  /* child */
        struct timespec start, end;
        gettime(&start);
        for (int i = 0; i < proc->exec_time; ++i) {
            unit_time();
        }
        gettime(&end);
        print_proc(getpid(), &start, &end);
        exit(0);
    }

    /* parent */
    printf("%s %d\n", proc->name, pid);
    // Fill pid into process struct
    proc->pid = pid;
    // Assign child process to another cpu
    assign_cpu(pid, PROC_CPU);
    // Lower the priority of child process(block it)
    set_priority(pid, LOW);
}

