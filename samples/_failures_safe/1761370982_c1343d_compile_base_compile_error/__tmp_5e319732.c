//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_MAX_SIZE 10
#define MATRIX_MIN_SIZE 1
#define MATRIX_ELEM_MAX 1000
#define MATRIX_ELEM_MIN -1000

#define MATRIX_GET(m, r, c, cols) ((m)[(r) * (cols) + (c)])
#define MATRIX_SET(m, r, c, cols, val) ((m)[(r) * (cols) + (c)] = (val))

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return value;
    }
}

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            MATRIX_SET(matrix, i, j, cols, 
                read_int("", MATRIX_ELEM_MIN, MATRIX_ELEM_MAX));
        }
    }
}

void print_matrix(const int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_GET(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(const int* a, int a_rows, int a_cols,
                      const int* b, int b_rows, int b_cols,
                      int* result) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                int elem_a = MATRIX_GET(a, i, k, a_cols);
                int elem_b = MATRIX_GET(b, k, j, b_cols);
                if ((elem_a > 0 && elem_b > 0 && elem_a > INT_MAX / elem_b) ||
                    (elem_a < 0 && elem_b < 0 && elem_a < INT_MAX / elem_b) ||
                    (elem_a > 0 && elem_b < 0 && elem_b < INT_MIN / elem_a) ||
                    (elem_a < 0 && elem_b > 0 && elem_a < INT_MIN / elem_b)) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                int product = elem_a * elem_b;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                sum += product;
            }
            MATRIX_SET(result, i, j, b_cols, sum);
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("Matrix dimensions must be between %d and %d\n", 
           MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    
    rows1 = read_int("Enter rows for matrix 1: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    rows2 = read_int("Enter rows for matrix 2: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of first matrix must equal rows of second matrix.\n");
        return 1;
    }
    
    int matrix1[MATRIX_MAX_SIZE * MATRIX_MAX_SIZE];
    int matrix2[MATRIX_MAX_SIZE * MATRIX_MAX_SIZE];
    int result[MATRIX_MAX_SIZE * MATRIX_MAX_SIZE];
    
    memset(matrix1, 0, sizeof(matrix1));
    memset(matrix2, 0, sizeof(matrix2));
    memset(result, 0, sizeof(result));
    
    printf("\nEnter elements for matrix 1:\n");
    fill_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter elements for matrix 2:\n");
    fill_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    multiply_matrices(matrix1, rows1, cols1, matrix2, rows