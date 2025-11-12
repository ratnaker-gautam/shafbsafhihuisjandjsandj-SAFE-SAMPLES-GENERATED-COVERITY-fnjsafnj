//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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

int read_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            if (scanf("%d", matrix + i * cols + j) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void add_matrices(int *result, int *a, int *b, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(result + i * cols + j) = *(a + i * cols + j) + *(b + i * cols + j);
        }
    }
}

void multiply_matrices(int *result, int *a, int a_rows, int a_cols, int *b, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            *(result + i * b_cols + j) = 0;
            for (int k = 0; k < a_cols; k++) {
                *(result + i * b_cols + j) += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];

    printf("Enter dimensions for first matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows1, &cols1) != 2 || rows1 <= 0 || cols1 <= 0 || rows1 > MAX_SIZE || cols1 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }

    printf("Enter first matrix:\n");
    if (!read_matrix((int *)matrix1, rows1, cols1)) {
        printf("Invalid input\n");
        return 1;
    }

    printf("Enter dimensions for second matrix (rows cols, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows2, &cols2) != 2 || rows2 <= 0 || cols2 <= 0 || rows2 > MAX_SIZE || cols2 > MAX_SIZE) {
        printf("Invalid dimensions\n");
        return 1;
    }

    printf("Enter second matrix:\n");
    if (!read_matrix((int *)matrix2, rows2, cols2)) {
        printf("Invalid input\n");
        return 1;
    }

    printf("\nMatrix 1:\n");
    print_matrix((int *)matrix1, rows1, cols1);

    printf("\nMatrix 2:\n");
    print_matrix((int *)matrix2, rows2, cols2);

    printf("\nMatrix Addition:\n");
    if (rows1 == rows2 && cols1 == cols2) {
        add_matrices((int *)result, (int *)matrix1, (int *)matrix2, rows1, cols1);
        print_matrix((int *)result, rows1, cols1);
    } else {
        printf("Cannot add - dimensions don't match\n");
    }

    printf("\nMatrix Multiplication:\n");
    if (cols1 == rows2) {
        multiply_matrices((int *)result, (int *)matrix1, rows1, cols1, (int *)matrix2, cols2);
        print_matrix((int *)result, rows1, cols2);
    } else {
        printf("Cannot multiply - inner dimensions don't match\n");
    }

    return 0;
}