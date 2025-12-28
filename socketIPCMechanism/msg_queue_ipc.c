#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

struct msg {
    long type;
    int n;
    int a[50];
};

int main() {
    key_t key = ftok("msg", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    struct msg m;
    int i;

    printf("Enter number of elements: ");
    scanf("%d", &m.n);

    printf("Enter elements:\n");
    for(i = 0; i < m.n; i++)
        scanf("%d", &m.a[i]);

    m.type = 1;

    printf("Before sorting:\n");
    for(i = 0; i < m.n; i++)
        printf("%d ", m.a[i]);
    printf("\n");

    msgsnd(msgid, &m, sizeof(m) - sizeof(long), 0);

    if(fork() == 0) {
        msgrcv(msgid, &m, sizeof(m) - sizeof(long), 1, 0);

        for(i = 0; i < m.n - 1; i++)
            for(int j = 0; j < m.n - i - 1; j++)
                if(m.a[j] > m.a[j + 1]) {
                    int t = m.a[j];
                    m.a[j] = m.a[j + 1];
                    m.a[j + 1] = t;
                }

        m.type = 2;
        msgsnd(msgid, &m, sizeof(m) - sizeof(long), 0);
        return 0;
    }

    wait(NULL);
    msgrcv(msgid, &m, sizeof(m) - sizeof(long), 2, 0);

    printf("After sorting:\n");
    for(i = 0; i < m.n; i++)
        printf("%d ", m.a[i]);
    printf("\n");

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
