#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

pthread_mutex_t lock;

void* handle(void* arg) {
    int sock = *(int*)arg;
    int choice, amount, balance;
    FILE *fp;

    read(sock, &choice, sizeof(choice));

    pthread_mutex_lock(&lock);
    fp = fopen("accountDB.txt", "r");
    fscanf(fp, "%d", &balance);
    fclose(fp);
    pthread_mutex_unlock(&lock);

    if(choice == 1) {
        read(sock, &amount, sizeof(amount));
        if(amount <= balance) balance -= amount;
    } else if(choice == 2) {
        read(sock, &amount, sizeof(amount));
        balance += amount;
    }

    pthread_mutex_lock(&lock);
    fp = fopen("accountDB.txt", "w");
    fprintf(fp, "%d", balance);
    fclose(fp);
    pthread_mutex_unlock(&lock);

    write(sock, &balance, sizeof(balance));
    close(sock);
    return NULL;
}

int main() {
    int s, ns;
    struct sockaddr_in addr;
    pthread_t t;

    pthread_mutex_init(&lock, NULL);

    s = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, 5);

    while(1) {
        ns = accept(s, NULL, NULL);
        pthread_create(&t, NULL, handle, &ns);
    }
}
