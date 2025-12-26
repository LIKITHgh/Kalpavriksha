#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    char name[20];
    int pid;
    int burst;
    int io_start;    
    int io_dur;       
    struct Process *next;
};

struct Process *front = NULL;
struct Process *rear = NULL;

void enqueue(struct Process *p) {
    if(front == NULL) {
        front = p;
        rear = p;
    } else {
        rear->next = p;
        rear = p;
    }
}

struct Process* dequeue() {
    if(front == NULL) {
        return NULL;
    }
    struct Process *temp = front;
    front = front->next;
    return temp;
}

int main() {

    int n;
    printf("Enter number of processes: ");
    fflush(stdout);
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Failed to read number of processes\n");
        return 1;
    }

    for(int i = 0; i < n; i++) {
        struct Process *p = (struct Process*) malloc(sizeof(struct Process));
        p->next = NULL;

        printf("Enter name pid burst io_start io_dur:\n");
        fflush(stdout);
        if (scanf("%19s %d %d %d %d",
              p->name, &p->pid, &p->burst, &p->io_start, &p->io_dur) != 5) {
            fprintf(stderr, "Failed to read process %d\n", i+1);
            free(p);
            return 1;
        }

        enqueue(p);
    }

    printf("\n--- FCFS SCHEDULING STARTED ---\n");

    struct Process *curr;

    while((curr = dequeue()) != NULL) {
        printf("\nRunning process: %s (PID %d)\n", curr->name, curr->pid);

        for(int t = 1; t <= curr->burst; t++) {
            printf("  Time %d: Executing...\n", t);
        }

        printf("Process %s finished.\n", curr->name);

        free(curr);
    }

    printf("\nAll processes executed.\n");

    return 0;
}
