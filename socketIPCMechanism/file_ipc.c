#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int n, a[50], i;
    FILE *fp;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter elements:\n");
    for(i = 0; i < n; i++)
        scanf("%d", &a[i]);

    printf("Before sorting:\n");
    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

    fp = fopen("data.txt", "w");
    fprintf(fp, "%d\n", n);
    for(i = 0; i < n; i++)
        fprintf(fp, "%d ", a[i]);
    fclose(fp);

    if(fork() == 0) {
        fp = fopen("data.txt", "r");
        fscanf(fp, "%d", &n);
        for(i = 0; i < n; i++)
            fscanf(fp, "%d", &a[i]);
        fclose(fp);

        for(i = 0; i < n - 1; i++)
            for(int j = 0; j < n - i - 1; j++)
                if(a[j] > a[j + 1]) {
                    int t = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = t;
                }

        fp = fopen("data.txt", "w");
        fprintf(fp, "%d\n", n);
        for(i = 0; i < n; i++)
            fprintf(fp, "%d ", a[i]);
        fclose(fp);
        return 0;
    }

    wait(NULL);

    fp = fopen("data.txt", "r");
    fscanf(fp, "%d", &n);
    for(i = 0; i < n; i++)
        fscanf(fp, "%d", &a[i]);
    fclose(fp);

    printf("After sorting:\n");
    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");

    return 0;
}
