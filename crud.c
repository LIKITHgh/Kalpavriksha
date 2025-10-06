#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "users.txt"

typedef struct {
    int id;
    char name[50];
    int age;
} User;

void createFileIfNotExists();
void addUser();
void displayUsers();
void updateUser();
void deleteUser();

int main() {
    int choice;

    createFileIfNotExists();

    while (1) {
        printf("\n===== User Management System =====\n");
        printf("1. Add User\n");
        printf("2. Display Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addUser(); break;
            case 2: displayUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void createFileIfNotExists() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error creating file!\n");
        exit(1);
    }
    fclose(file);
}

void addUser() {
    User u;
    FILE *file = fopen(FILENAME, "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &u.id);
    printf("Enter Name: ");
    scanf(" %[^\n]", u.name); 
    printf("Enter Age: ");
    scanf("%d", &u.age);

    fprintf(file, "%d %s %d\n", u.id, u.name, u.age);
    fclose(file);

    printf("User added successfully!\n");
}

void displayUsers() {
    User u;
    FILE *file = fopen(FILENAME, "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n===== User Records =====\n");
    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) != EOF) {
        printf("ID: %d | Name: %s | Age: %d\n", u.id, u.name, u.age);
    }
    fclose(file);
}

void updateUser() {
    User u;
    int id, found = 0;
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) != EOF) {
        if (u.id == id) {
            printf("Enter new Name: ");
            scanf(" %[^\n]", u.name);
            printf("Enter new Age: ");
            scanf("%d", &u.age);
            found = 1;
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("User updated successfully!\n");
    else
        printf("User with ID %d not found!\n", id);
}

void deleteUser() {
    User u;
    int id, found = 0;
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) != EOF) {
        if (u.id != id) {
            fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("User deleted successfully!\n");
    else
        printf("User with ID %d not found!\n", id);
}
