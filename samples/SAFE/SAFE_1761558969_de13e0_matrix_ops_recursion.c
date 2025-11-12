//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    if (scanf("%d %d", rows, cols) != 2) return 0;
    if (*rows <= 0 || *cols <= 0 || *rows > MAX_SIZE || *cols > MAX_SIZE) return 0;
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_recursive(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int i, int j, int rows, int cols) {
    if (i >= rows) return;
    if (j >= cols) {
        transpose_recursive(src, dst, i + 1, 0, rows, cols);
        return;
    }
    dst[j][i] = src[i][j];
    transpose_recursive(src, dst, i, j + 1, rows, cols);
}

int multiply_recursive(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int c[MAX_SIZE][MAX_SIZE], int i, int j, int k, int arows, int acols, int bcols) {
    if (i >= arows) return 1;
    if (j >= bcols) {
        return multiply_recursive(a, b, c, i + 1, 0, 0, arows, acols, bcols);
    }
    if (k >= acols) {
        return multiply_recursive(a, b, c, i, j + 1, 0, arows, acols, bcols);
    }
    c[i][j] += a[i][k] * b[k][j];
    return multiply_recursive(a, b, c, i, j, k + 1, arows, acols, bcols);
}

int main() {
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (choice == 1) {
        int matrix[MAX_SIZE][MAX_SIZE];
        int transposed[MAX_SIZE][MAX_SIZE];
        int rows, cols;
        if (!read_matrix(matrix, &rows, &cols)) {
            printf("Invalid matrix input\n");
            return 1;
        }
        memset(transposed, 0, sizeof(transposed));
        transpose_recursive(matrix, transposed, 0, 0, rows, cols);
        print_matrix(transposed, cols, rows);
    } else if (choice == 2) {
        int a[MAX_SIZE][MAX_SIZE];
        int b[MAX_SIZE][MAX_SIZE];
        int c[MAX_SIZE][MAX_SIZE];
        int arows, acols, brows, bcols;
        if (!read_matrix(a, &arows, &acols)) {
            printf("Invalid matrix A input\n");
            return 1;
        }
        if (!read_matrix(b, &brows, &bcols)) {
            printf("Invalid matrix B input\n");
            return 1;
        }
        if (acols != brows) {
            printf("Matrix dimensions incompatible\n");
            return 1;
        }
        memset(c, 0, sizeof(c));
        if (!multiply_recursive(a, b, c, 0, 0, 0, arows, acols, bcols)) {
            printf("Multiplication failed\n");
            return 1;
        }
        print_matrix(c, arows, bcols);
    } else {
        printf("Invalid choice\n");
        return 1;
    }

    return 0;
}