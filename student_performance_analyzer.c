#include <stdio.h>
#include <string.h>

struct Student {
    int roll;
    char name[50];
    float marks[3];
    float total;
    float average;
    char grade;
};

float calculateTotal(float marks[]);
float calculateAverage(float total);
char assignGrade(float avg);
void displayPerformance(char grade);
void printRollNumbersRecursively(int roll, int n);

int totalStudents = 0; 


float calculateTotal(float marks[]) {
    float total = 0;
    for (int i = 0; i < 3; i++) {
        total += marks[i];
    }
    return total;
}

float calculateAverage(float total) {
    return total / 3.0;
}

char assignGrade(float avg) {
    if (avg >= 85) return 'A';
    else if (avg >= 70) return 'B';
    else if (avg >= 50) return 'C';
    else if (avg >= 35) return 'D';
    else return 'F';
}

void displayPerformance(char grade) {
    int stars;
    switch (grade) {
        case 'A': stars = 5; break;
        case 'B': stars = 4; break;
        case 'C': stars = 3; break;
        case 'D': stars = 2; break;
        default: stars = 0; break;
    }
    for (int i = 0; i < stars; i++) {
        printf("*");
    }
}

void printRollNumbersRecursively(int roll, int n) {
    if (roll > n) return;
    printf("%d ", roll);
    printRollNumbersRecursively(roll + 1, n);
}

int main() {
    struct Student s[100];
    int n;

    printf("Enter number of students: ");
    scanf("%d", &n);

    if (n < 1 || n > 100) {
        printf("Invalid number of students. Must be between 1 and 100.\n");
        return 0;
    }

    totalStudents = n; 

    printf("Enter details: Roll Name Marks1 Marks2 Marks3\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %s %f %f %f", &s[i].roll, s[i].name, &s[i].marks[0], &s[i].marks[1], &s[i].marks[2]);
        s[i].total = calculateTotal(s[i].marks);
        s[i].average = calculateAverage(s[i].total);
        s[i].grade = assignGrade(s[i].average);
    }

    printf("\n----- Student Performance Report -----\n\n");
    for (int i = 0; i < n; i++) {
        printf("Roll: %d\n", s[i].roll);
        printf("Name: %s\n", s[i].name);
        printf("Total: %.0f\n", s[i].total);
        printf("Average: %.2f\n", s[i].average);
        printf("Grade: %c\n", s[i].grade);

        if (s[i].average < 35) {
            printf("\n");
            continue;  
        }

        printf("Performance: ");
        displayPerformance(s[i].grade);
        printf("\n\n");
    }

    printf("List of Roll Numbers (via recursion): ");
    printRollNumbersRecursively(1, n);
    printf("\n");

    return 0;
}