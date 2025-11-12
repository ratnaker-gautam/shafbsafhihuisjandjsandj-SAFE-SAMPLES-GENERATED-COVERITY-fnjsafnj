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

#define SAFE_MULT(a, b, result) \
    do { \
        if ((a) > 0 && (b) > 0 && (a) > INT_MAX / (b)) { \
            *(result) = 0; \
            return 0; \
        } \
        if ((a) < 0 && (b) < 0 && (a) < INT_MAX / (b)) { \
            *(result) = 0; \
            return 0; \
        } \
        if ((a) > 0 && (b) < 0 && (b) < INT_MIN / (a)) { \
            *(result) = 0; \
            return 0; \
        } \
        if ((a) < 0 && (b) > 0 && (a) < INT_MIN / (b)) { \
            *(result) = 0; \
            return 0; \
        } \
        *(result) = (a) * (b); \
    } while(0)

int read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    char input[256];
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", rows) != 1) return 0;
    if (*rows < 1 || *rows > MAX_SIZE) return 0;
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", cols) != 1) return 0;
    if (*cols < 1 || *cols > MAX_SIZE) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            if (sscanf(input, "%d", &matrix[i][j]) != 1) return 0;
            if (matrix[i][j] < MATRIX_ELEMENT_MIN || matrix[i][j] > MATRIX_ELEMENT_MAX) return 0;
        }
    }
    return 1;
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(int A[MAX_SIZE][MAX_SIZE], int B[MAX_SIZE][MAX_SIZE], 
               int C[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((A[i][j] > 0 && B[i][j] > INT_MAX - A[i][j]) ||
                (A[i][j] < 0 && B[i][j] < INT_MIN - A[i][j])) {
                return 0;
            }
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return 1;
}

int matrix_subtract(int A[MAX_SIZE][MAX_SIZE], int B[MAX_SIZE][MAX_SIZE], 
                    int C[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((B[i][j] > 0 && A[i][j] < INT_MIN + B[i][j]) ||
                (B[i][j] < 0 && A[i][j] > INT_MAX + B[i][j])) {
                return 0;
            }
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return 1;
}

int matrix_multiply(int A[MAX_SIZE][MAX_SIZE], int B[MAX_SIZE][MAX_SIZE], 
                    int C[MAX_SIZE][MAX_SIZE], int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            C[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                int product;
                SAFE_MULT(A[i][k], B[k][j], &product);
                
                if ((product > 0 && C[i][j] > INT_MAX - product) ||
                    (product < 0 && C[i][j] < INT_MIN - product)) {
                    return 0;
                }
                C[i][j] +=