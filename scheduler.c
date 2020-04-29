#include "common.h"
#include <stdio.h>
#include <stdlib.h>

// ========================================
// Ready queue
struct list {
    struct list *prev;
    struct list *next;
    int proc_idx;
};
static int rq_nitems = 0;  // Number of items in the queue
static struct list list_head = {NULL, NULL, -1};  // Sentinel node
static struct list list_tail = {NULL, NULL, -1};  // Sentinel node

void initilalize_list() {
    list_head.next = &list_tail;
    list_tail.prev = &list_head;
}

void push_front(int proc_idx)
{
    struct list *node = (struct list *) malloc(sizeof(struct list));
    node->prev = &list_head;
    node->next = list_head.next;
    node->proc_idx = proc_idx;
    list_head.next->prev = node;
    list_head.next = node;
    ++rq_nitems;
}

void push_back(int proc_idx)
{
    struct list *node = (struct list *) malloc(sizeof(struct list));
    node->prev = list_tail.prev;
    node->next = &list_tail;
    node->proc_idx = proc_idx;
    list_tail.prev->next = node;
    list_tail.prev = node;
    ++rq_nitems;
}

int pop_front()
{
    struct list *node = list_head.next;
    int proc_idx = node->proc_idx;
    list_head.next = node->next;
    node->next->prev = &list_head;
    free(node);
    --rq_nitems;
    return proc_idx;
}

int pop_back()
{
    struct list *node = list_tail.prev;
    int proc_idx = node->proc_idx;
    list_tail.prev = node->prev;
    node->prev->next = &list_tail;
    free(node);
    --rq_nitems;
    return proc_idx;
}

void delete_node(struct list* node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    --rq_nitems;
    free(node);
}

// ========================================

// compare based on ready time
int _cmp(const void *a, const void *b)
{
    return ((struct process *)a)->ready_time -
           ((struct process *)b)->ready_time;
}

int next_process_to_run(int policy, struct process *procs)
{
    struct list *node, *min_node;
    int exec_time, min_exec_time, proc_idx;
    switch (policy) {
        case FIFO:
        case RR:
            return pop_front();
        case SJF:
        case PSJF:
            node = min_node = list_head.next;
            exec_time = min_exec_time = procs[node->proc_idx].exec_time;
            while (node->next->next != NULL) {
                node = node->next;
                exec_time = procs[node->proc_idx].exec_time;
                if (exec_time < min_exec_time) {
                    min_exec_time = exec_time;
                    min_node = node;
                }
            }
            proc_idx = min_node->proc_idx;
            delete_node(min_node);
            return proc_idx;

        default:
            fprintf(stderr, "Invalid policy: %d\n", policy);
            exit(1);
    }
}

void run_processes(int policy, int n_procs, struct process *procs)
{
    // Sort processes based on ready time
    qsort(procs, n_procs, sizeof(struct process), _cmp);

    int cur_time = 0;           // Current timestamp
    int running_proc_idx = -1;  // The index of current running process
                                // (-1 means no processes running)
    int next_fork_idx = 0;      // The index of next unforked process
    int time_quantum = TQ;      // Time quantum for RR
    int n_finished_procs = 0;   // Number of finished processes

    initilalize_list();

    while (n_finished_procs != n_procs) {
        // Forking processes
        for (int i = next_fork_idx; i < n_procs; ++i) {
            if (procs[i].ready_time == cur_time) {
                fork_process(procs + i);
                if (running_proc_idx != -1 &&  // A process is running
                    policy == PSJF &&          // policy is PSJF
                    // And remaining exec_time > new proc
                    procs[running_proc_idx].exec_time > procs[i].exec_time) {
                    // Block running process
                    set_priority(procs[running_proc_idx].pid, LOW);
                    push_back(running_proc_idx);
                    running_proc_idx = -1;
                }
                push_back(i);
                ++next_fork_idx;
            }
        }

        if (running_proc_idx != -1 &&                 // A process is running
            procs[running_proc_idx].exec_time == 0) { // but finished
            ++n_finished_procs;
            running_proc_idx = -1;
        }
        if (running_proc_idx != -1 &&   // A process is running
            policy == RR &&             // policy is RR
            time_quantum < 0) {         // time quantum reached
            // Block the process
            set_priority(procs[running_proc_idx].pid, LOW);
            push_back(running_proc_idx);
            // Replenish the time quantum
            time_quantum = TQ;
            running_proc_idx = -1;
        }
        if (running_proc_idx == -1 &&   // No processes running
            rq_nitems != 0){            // ready queue is not empty
            // Pick a process from ready queue to run
            running_proc_idx = next_process_to_run(policy, procs);
            set_priority(procs[running_proc_idx].pid, HIGH);
        }

        // Run a unit of time
        unit_time();
        ++cur_time;
        if (running_proc_idx != -1) {
            --procs[running_proc_idx].exec_time;
        }
        if (policy == RR) {
            --time_quantum;
        }
    }
}

