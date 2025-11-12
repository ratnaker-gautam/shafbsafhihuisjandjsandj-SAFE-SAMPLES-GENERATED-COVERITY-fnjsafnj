//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void add_matrices(int *a, int *b, int *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + i * cols + j) = *(a + i * cols + j) + *(b + i * cols + j);
        }
    }
}

void multiply_matrices(int *a, int a_rows, int a_cols, int *b, int b_rows, int b_cols, int *result) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
}

int read_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (scanf("%d", matrix + i * cols + j) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE * MAX_SIZE];
    int matrix2[MAX_SIZE * MAX_SIZE];
    int result[MAX_SIZE * MAX_SIZE];

    printf("Matrix Operations\n");
    printf("1. Add matrices\n");
    printf("2. Multiply matrices\n");
    printf("Enter choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        return 1;
    }

    printf("Enter rows and columns for first matrix: ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    if (rows1 <= 0 || rows1 > MAX_SIZE || cols1 <= 0 || cols1 > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    if (choice == 1) {
        printf("Enter rows and columns for second matrix: ");
        if (scanf("%d %d", &rows2, &cols2) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
            printf("Invalid matrix dimensions\n");
            return 1;
        }
        if (rows1 != rows2 || cols1 != cols2) {
            printf("Matrix dimensions must match for addition\n");
            return 1;
        }
    } else {
        printf("Enter rows and columns for second matrix: ");
        if (scanf("%d %d", &rows2, &cols2) != 2) {
            printf("Invalid input\n");
            return 1;
        }
        if (rows2 <= 0 || rows2 > MAX_SIZE || cols2 <= 0 || cols2 > MAX_SIZE) {
            printf("Invalid matrix dimensions\n");
            return 1;
        }
        if (cols1 != rows2) {
            printf("Matrix dimensions incompatible for multiplication\n");
            return 1;
        }
    }

    printf("Enter elements of first matrix:\n");
    if (!read_matrix(matrix1, rows1, cols1)) {
        printf("Invalid matrix input\n");
        return 1;
    }

    printf("Enter elements of second matrix:\n");
    if (!read_matrix(matrix2, rows2, cols2)) {
        printf("Invalid matrix input\n");
        return 1;
    }

    printf("\nFirst matrix:\n");
    print_matrix(matrix1, rows1, cols1);
    printf("\nSecond matrix:\n");
    print_matrix(matrix2, rows2, cols2);

    if (choice == 1) {
        add_matrices(matrix1, matrix2, result, rows1, cols1);
        printf("\nSum matrix:\n");
        print_matrix(result, rows1, cols1);
    } else {
        multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, result);
        printf("\nProduct matrix:\n");
        print_matrix(result, rows1, cols2);
    }

    return 0;
}