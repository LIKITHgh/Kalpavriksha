#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 50

typedef struct FreeBlock {
    int idx;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct FileNode {
    char name[MAX_NAME];
    bool isDir;
    int *blocks;
    int blockCount;
    int size;
    struct FileNode *parent;
    struct FileNode *child;
    struct FileNode *next;
    struct FileNode *prev;
} FileNode;

unsigned char disk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlock *freeHead = NULL;
FreeBlock *freeTail = NULL;
int freeCount = 0;
FileNode *root = NULL;
FileNode *cwd = NULL;

void addBlockTail(int index) {
    FreeBlock *node = malloc(sizeof(FreeBlock));
    node->idx = index;
    node->next = NULL;
    node->prev = freeTail;
    if (freeTail != NULL) {
        freeTail->next = node;
    }
    freeTail = node;
    if (freeHead == NULL) {
        freeHead = node;
    }
    freeCount++;
}

int allocBlocks(int n, int *out) {
    if (n > freeCount) {
        return -1;
    }
    for (int i = 0; i < n; i++) {
        FreeBlock *tmp = freeHead;
        out[i] = tmp->idx;
        freeHead = tmp->next;
        if (freeHead != NULL) {
            freeHead->prev = NULL;
        } else {
            freeTail = NULL;
        }
        free(tmp);
        freeCount--;
    }
    return 0;
}

void freeBlocks(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        addBlockTail(arr[i]);
    }
}

FileNode *newNode(char *name, bool isDir, FileNode *parent) {
    FileNode *n = calloc(1, sizeof(FileNode));
    strcpy(n->name, name);
    n->isDir = isDir;
    n->parent = parent;
    n->child = NULL;
    n->next = NULL;
    n->prev = NULL;
    n->blocks = NULL;
    n->blockCount = 0;
    n->size = 0;
    return n;
}

FileNode *find(FileNode *dir, char *name) {
    FileNode *c = dir->child;
    while (c != NULL) {
        if (strcmp(c->name, name) == 0) {
            return c;
        }
        c = c->next;
    }
    return NULL;
}

void addChild(FileNode *dir, FileNode *child) {
    if (dir->child == NULL) {
        dir->child = child;
    } else {
        FileNode *t = dir->child;
        while (t->next != NULL) {
            t = t->next;
        }
        t->next = child;
        child->prev = t;
    }
}

void removeChild(FileNode *n) {
    if (n->prev != NULL) {
        n->prev->next = n->next;
    } else {
        n->parent->child = n->next;
    }
    if (n->next != NULL) {
        n->next->prev = n->prev;
    }
}

void mkdirCmd(char *name) {
    if (find(cwd, name) != NULL) {
        printf("Directory exists\n");
        return;
    }
    FileNode *d = newNode(name, true, cwd);
    addChild(cwd, d);
    printf("Directory '%s' created\n", name);
}

void createCmd(char *name) {
    if (find(cwd, name) != NULL) {
        printf("File exists\n");
        return;
    }
    FileNode *f = newNode(name, false, cwd);
    addChild(cwd, f);
    printf("File '%s' created\n", name);
}

void writeCmd(char *name, char *text) {
    FileNode *f = find(cwd, name);
    if (f == NULL || f->isDir) {
        printf("Invalid file\n");
        return;
    }
    if (f->blocks != NULL) {
        freeBlocks(f->blocks, f->blockCount);
        free(f->blocks);
        f->blocks = NULL;
    }
    int len = strlen(text);
    int need = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;
    f->blocks = malloc(need * sizeof(int));
    if (allocBlocks(need, f->blocks) != 0) {
        printf("No space left\n");
        return;
    }
    f->size = len;
    f->blockCount = need;
    int remaining = len;
    int offset = 0;
    for (int i = 0; i < need; i++) {
        int copy = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        memcpy(disk[f->blocks[i]], text + offset, copy);
        remaining -= copy;
        offset += copy;
    }
    printf("Data written to '%s'\n", name);
}

void readCmd(char *name) {
    FileNode *f = find(cwd, name);
    if (f == NULL || f->isDir || f->blocks == NULL) {
        printf("Invalid file\n");
        return;
    }
    int remaining = f->size;
    for (int i = 0; i < f->blockCount && remaining > 0; i++) {
        int copy = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        fwrite(disk[f->blocks[i]], 1, copy, stdout);
        remaining -= copy;
    }
    printf("\n");
}

void deleteCmd(char *name) {
    FileNode *f = find(cwd, name);
    if (f == NULL || f->isDir) {
        printf("Invalid file\n");
        return;
    }
    if (f->blocks != NULL) {
        freeBlocks(f->blocks, f->blockCount);
        free(f->blocks);
    }
    removeChild(f);
    free(f);
    printf("File '%s' deleted\n", name);
}

void rmdirCmd(char *name) {
    FileNode *f = find(cwd, name);
    if (f == NULL || !f->isDir || f->child != NULL) {
        printf("Invalid directory\n");
        return;
    }
    removeChild(f);
    free(f);
    printf("Directory '%s' removed\n", name);
}

void lsCmd() {
    FileNode *c = cwd->child;
    if (c == NULL) {
        printf("(empty)\n");
        return;
    }
    while (c != NULL) {
        printf("%s%s\n", c->name, c->isDir ? "/" : "");
        c = c->next;
    }
}

void cdCmd(char *name) {
    if (strcmp(name, "/") == 0) {
        cwd = root;
        return;
    }
    if (strcmp(name, "..") == 0) {
        if (cwd != root) {
            cwd = cwd->parent;
        }
        return;
    }
    FileNode *d = find(cwd, name);
    if (d == NULL || !d->isDir) {
        printf("Invalid directory\n");
        return;
    }
    cwd = d;
}

void pwdCmd(FileNode *n) {
    if (n == root) {
        printf("/");
        return;
    }
    pwdCmd(n->parent);
    printf("%s/", n->name);
}

void dfCmd() {
    printf("Free: %d  Used: %d\n", freeCount, NUM_BLOCKS - freeCount);
}

int main() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        addBlockTail(i);
    }
    root = newNode("/", true, NULL);
    cwd = root;
    printf("VFS ready. Type 'exit' to quit.\n");
    char cmd[1000];
    char arg1[100];
    char arg2[900];
    while (1) {
        printf("> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) {
            break;
        }
        cmd[strcspn(cmd, "\n")] = 0;
        if (strcmp(cmd, "exit") == 0) {
            break;
        }
        if (sscanf(cmd, "mkdir %s", arg1) == 1) {
            mkdirCmd(arg1);
        } else if (sscanf(cmd, "create %s", arg1) == 1) {
            createCmd(arg1);
        } else if (sscanf(cmd, "write %s \"%[^\"]\"", arg1, arg2) == 2) {
            writeCmd(arg1, arg2);
        } else if (sscanf(cmd, "read %s", arg1) == 1) {
            readCmd(arg1);
        } else if (sscanf(cmd, "delete %s", arg1) == 1) {
            deleteCmd(arg1);
        } else if (sscanf(cmd, "rmdir %s", arg1) == 1) {
            rmdirCmd(arg1);
        } else if (strcmp(cmd, "ls") == 0) {
            lsCmd();
        } else if (sscanf(cmd, "cd %s", arg1) == 1) {
            cdCmd(arg1);
        } else if (strcmp(cmd, "pwd") == 0) {
            pwdCmd(cwd);
            printf("\n");
        } else if (strcmp(cmd, "df") == 0) {
            dfCmd();
        } else {
            printf("Unknown command\n");
        }
    }
    printf("Exiting...\n");
    return 0;
}
