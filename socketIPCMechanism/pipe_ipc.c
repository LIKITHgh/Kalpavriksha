#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd1[2], fd2[2], n, a[50], i;

    pipe(fd1);
    pipe(fd2);

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter elements:\n");
    for(i = 0; i < n; i++)
        scanf("%d", &a[i]);

    printf("Before sorting:\n");
    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

    if(fork() == 0) {
        read(fd1[0], &n, sizeof(n));
        read(fd1[0], a, sizeof(int) * n);

        for(i = 0; i < n - 1; i++)
            for(int j = 0; j < n - i - 1; j++)
                if(a[j] > a[j + 1]) {
                    int t = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = t;
                }

        write(fd2[1], a, sizeof(int) * n);
        return 0;
    }

    write(fd1[1], &n, sizeof(n));
    write(fd1[1], a, sizeof(int) * n);

    wait(NULL);
    read(fd2[0], a, sizeof(int) * n);

    printf("After sorting:\n");
    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

    return 0;
}
