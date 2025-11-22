#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;
    char data[50];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    int cap;
    int size;
    Node **hash;
    Node *head;
    Node *tail;
} LRUCache;

Node* makeNode(int key, char *value){
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = key;
    strcpy(n->data, value);
    n->prev = NULL;
    n->next = NULL;
    return n;
}

void moveToFront(LRUCache *c, Node *n){
    if (c->head == n) return;

    if (n->prev) n->prev->next = n->next;
    if (n->next) n->next->prev = n->prev;

    if (c->tail == n)
        c->tail = n->prev;

    n->next = c->head;
    n->prev = NULL;

    if (c->head)
        c->head->prev = n;

    c->head = n;

    if (c->tail == NULL)
        c->tail = n;
}

void removeLRU(LRUCache *c){
    if (c->tail == NULL) return;
    Node *temp = c->tail;

    if (temp->prev)
        temp->prev->next = NULL;
    c->tail = temp->prev;
    c->hash[temp->key] = NULL;
    free(temp);
    c->size--;
}

LRUCache* createCache(int capacity){
    LRUCache *c = (LRUCache*)malloc(sizeof(LRUCache));
    c->cap = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;

    c->hash = (Node**)calloc(10000, sizeof(Node*));
    return c;
}

char* get(LRUCache *c, int key){
    Node *n = c->hash[key];

    if (n == NULL)
        return NULL;
    moveToFront(c, n);
    return n->data;
}

void put(LRUCache *c, int key, char *value){
    Node *n = c->hash[key];

    if (n != NULL) {
        strcpy(n->data, value);
        moveToFront(c, n);
        return;
    }

    if (c->size >= c->cap) {
        removeLRU(c);
    }

    Node *newNode = makeNode(key, value);
    c->hash[key] = newNode;
    if (c->head == NULL) {
        c->head = c->tail = newNode;
    } else {
        newNode->next = c->head;
        c->head->prev = newNode;
        c->head = newNode;
    }
    c->size++;
}

int main(){
    LRUCache *cache;
    char cmd[20], value[50];
    int key, size;

    while (1){
        scanf("%s", cmd);
        if (strcmp(cmd, "createCache") == 0){
            scanf("%d", &size);
            cache = createCache(size);
        }
        else if (strcmp(cmd, "put") == 0){
            scanf("%d %s", &key, value);
            put(cache, key, value);
        }
        else if (strcmp(cmd, "get") == 0){
            scanf("%d", &key);
            char *res = get(cache, key);
            if (res == NULL) printf("NULL\n");
            else printf("%s\n", res);
        }
        else if (strcmp(cmd, "exit") == 0){
            break;
        }
    }

    return 0;
}