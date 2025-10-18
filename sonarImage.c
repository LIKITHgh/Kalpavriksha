#include <stdio.h>
#include <stdlib.h>

void inputMatrix(int *matrix, int n);
void displayMatrix(int *matrix, int n);
void rotateMatrix90Clockwise(int *matrix, int n);
void applySmoothingFilter(int *matrix, int n);

int main() {
    int n;

    printf("Enter matrix size (2-10): ");
    scanf("%d", &n);

    int *matrix = (int *)malloc(n * n * sizeof(int));
    printf("\nEnter elements of the matrix:\n");
    inputMatrix(matrix, n);

    printf("\nOriginal Matrix:\n");
    displayMatrix(matrix, n);

    printf("\nMatrix after Clockwise Rotation:\n");
    rotateMatrix90Clockwise(matrix, n);
    displayMatrix(matrix, n);

    printf("\nMatrix after Applying Smoothing Filter:\n");
    applySmoothingFilter(matrix, n);
    displayMatrix(matrix, n);

    return 0;
}

void inputMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("Enter value for element [%d][%d]: ", i, j);
            scanf("%d", (matrix + i * n + j));
        }
    }
}

void displayMatrix(int *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", *(matrix + i * n + j));
        }
        printf("\n");
    }
}

void rotateMatrix90Clockwise(int *matrix, int n) {
    for (int layer = 0; layer < n / 2; layer++) {
        int first = layer;
        int last = n - 1 - layer;

        for (int i = first; i < last; i++) {
            int offset = i - first;
            int top = *(matrix + first * n + i);

            // LeftTop
            *(matrix + first * n + i) = *(matrix + (last - offset) * n + first);
            // BottomLeft
            *(matrix + (last - offset) * n + first) = *(matrix + last * n + (last - offset));
            // RightBottom
            *(matrix + last * n + (last - offset)) = *(matrix + i * n + last);
            // TopRight
            *(matrix + i * n + last) = top;
        }
    }
}

void applySmoothingFilter(int *matrix, int n) {
    int *tempRow = (int *)malloc(n * sizeof(int));

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

}
