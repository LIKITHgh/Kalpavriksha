#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void inputMatrix(int *matrix, int n);
void randomMatrix(int *matrix, int n);
void displayMatrix(int *matrix, int n);
void rotateMatrix90Clockwise(int *matrix, int n);
void applySmoothingFilter(int *matrix, int n);

int main() {
    int n, choice;

    do {
        printf("Enter matrix size (2–10): ");
        scanf("%d", &n);
        if (n < 2 || n > 10)
            printf("Invalid size! Please enter a value between 2 and 10.\n");
    } while (n < 2 || n > 10);

    int *matrix = (int *)malloc(n * n * sizeof(int));
    if (!matrix) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("\nChoose how to fill the matrix:\n");
    printf("1. Enter manually\n");
    printf("2. Generate random matrix\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
        inputMatrix(matrix, n);
    else if (choice == 2)
        randomMatrix(matrix, n);
    else {
        printf("Invalid choice! Defaulting to manual input.\n");
        inputMatrix(matrix, n);
    }

    printf("\nOriginal Matrix:\n");
    displayMatrix(matrix, n);

    printf("\nMatrix after Clockwise Rotation:\n");
    rotateMatrix90Clockwise(matrix, n);
    displayMatrix(matrix, n);

    printf("\nMatrix after Applying Smoothing Filter:\n");
    applySmoothingFilter(matrix, n);
    displayMatrix(matrix, n);

    free(matrix);
    return 0;
}

// input
void inputMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("Enter value for element [%d][%d]: ", i, j);
            scanf("%d", (matrix + i * n + j));
        }
    }
}

void randomMatrix(int *matrix, int n) {
    srand(time(NULL));
    printf("\nGenerating random matrix...\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            *(matrix + i * n + j) = rand() % 100;  
        }
    }
}

// Display matrix
void displayMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", *(matrix + i * n + j));
        }
        printf("\n");
    }
}

// Rotate 90° clockwise
void rotateMatrix90Clockwise(int *matrix, int n) {
    for (int layer = 0; layer < n / 2; layer++) {
        int first = layer;
        int last = n - 1 - layer;
        for (int i = first; i < last; i++) {
            int offset = i - first;
            int top = *(matrix + first * n + i);
            *(matrix + first * n + i) = *(matrix + (last - offset) * n + first);
            *(matrix + (last - offset) * n + first) = *(matrix + last * n + (last - offset));
            *(matrix + last * n + (last - offset)) = *(matrix + i * n + last);
            *(matrix + i * n + last) = top;
        }
    }
}

// Smoothing filter
void applySmoothingFilter(int *matrix, int n) {
    int *tempRow = (int *)malloc(n * sizeof(int));
    if (!tempRow) return;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0, count = 0;
            for (int x = i - 1; x <= i + 1; x++) {
                for (int y = j - 1; y <= j + 1; y++) {
                    if (x >= 0 && x < n && y >= 0 && y < n) {
                        sum += *(matrix + x * n + y);
                        count++;
                    }
                }
            }
            *(tempRow + j) = sum / count;
        }
        for (int j = 0; j < n; j++) {
            *(matrix + i * n + j) = *(tempRow + j);
        }
    }

    free(tempRow);
}
