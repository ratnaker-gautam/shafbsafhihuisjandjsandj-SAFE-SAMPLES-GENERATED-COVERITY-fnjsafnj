//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define SWAP(a, b) do { \
    int temp = (a); \
    (a) = (b); \
    (b) = temp; \
} while(0)

#define PRINT_MATRIX(mat, rows, cols) do { \
    for (int i = 0; i < (rows); i++) { \
        for (int j = 0; j < (cols); j++) { \
            printf("%6d", (mat)[i][j]); \
        } \
        printf("\n"); \
    } \
} while(0)

#define VALIDATE_SIZE(n) ((n) > 0 && (n) <= MAX_SIZE)

int read_matrix(int rows, int cols, int matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                return 0;
            }
            char *endptr;
            long val = strtol(buffer, &endptr, 10);
            if (endptr == buffer || *endptr != '\n') {
                return 0;
            }
            if (val < MATRIX_ELEMENT_MIN || val > MATRIX_ELEMENT_MAX) {
                return 0;
            }
            matrix[i][j] = (int)val;
        }
    }
    return 1;
}

int matrix_add(int rows, int cols, int a[MAX_SIZE][MAX_SIZE], 
               int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long sum = (long)a[i][j] + (long)b[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                return 0;
            }
            result[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_subtract(int rows, int cols, int a[MAX_SIZE][MAX_SIZE],
                    int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long diff = (long)a[i][j] - (long)b[i][j];
            if (diff > INT_MAX || diff < INT_MIN) {
                return 0;
            }
            result[i][j] = (int)diff;
        }
    }
    return 1;
}

int matrix_multiply(int a_rows, int a_cols, int b_rows, int b_cols,
                    int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                    int result[MAX_SIZE][MAX_SIZE]) {
    if (a_cols != b_rows) {
        return 0;
    }
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            long sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)a[i][k] * (long)b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    return 0;
                }
            }
            result[i][j] = (int)sum;
        }
    }
    return 1;
}

int main() {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    
    printf("Enter dimensions for first matrix (rows columns): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    if (!VALIDATE_SIZE(rows1) || !VALIDATE_SIZE(cols1)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter first matrix elements:\n");
    if (!read_matrix(rows1, cols1, matrix1)) {
        printf("Invalid matrix input\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows columns): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    if (!VALIDATE_SIZE(rows2) || !VALIDATE_SIZE(cols2)) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    
    printf("Enter second matrix elements:\n");
    if (!read_matrix(rows2, cols2, matrix2)) {
        printf