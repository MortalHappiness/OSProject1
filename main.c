#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========================================

int main(int argc, char *argv[])
{
    int policy, n_procs;
    char buf[32];
    struct process *procs;

    scanf("%s", buf);
    if (strcmp(buf, "FIFO") == 0) {
        policy = FIFO;
    } else if (strcmp(buf, "RR") == 0) {
        policy = RR;
    } else if (strcmp(buf, "SJF") == 0) {
        policy = SJF;
    } else if (strcmp(buf, "PSJF") == 0) {
        policy = PSJF;
    } else {
        fprintf(stderr, "Invalid policy name: %s\n", buf);
        exit(1);
    }

    scanf("%d", &n_procs);
    procs = (struct process *) malloc(n_procs * sizeof(struct process));
    for (int i = 0; i < n_procs; ++i) {
        scanf("%s%d%d",
              procs[i].name,
              &(procs[i].ready_time),
              &(procs[i].exec_time));
    }

    // Assign main process to cpu
    assign_cpu(0, SCHED_CPU);
    // Raise the priority of main process
    set_priority(0, HIGH);

    // Run child processes and let scheduler manage them
    run_processes(policy, n_procs, procs);

    free(procs);

    return 0;
}

