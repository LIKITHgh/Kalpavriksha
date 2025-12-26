#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int shmid = shmget(IPC_PRIVATE, 1024, 0666 | IPC_CREAT);
    int *a = (int*)shmat(shmid, NULL, 0);
    int n, i;

    printf("Enter number of elements: ");
    scanf("%d", &n);
    a[0] = n;

    printf("Enter elements:\n");
    for(i = 0; i < n; i++)
        scanf("%d", &a[i + 1]);

    printf("Before sorting:\n");
    for(i = 0; i < n; i++)
        printf("%d ", a[i + 1]);
    printf("\n");

    if(fork() == 0) {
        for(i = 1; i < n; i++)
            for(int j = 1; j <= n - i; j++)
                if(a[j] > a[j + 1]) {
                    int t = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = t;
                }
        return 0;
    }

    wait(NULL);

    printf("After sorting:\n");
    for(i = 0; i < n; i++)
        printf("%d ", a[i + 1]);
    printf("\n");

    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
