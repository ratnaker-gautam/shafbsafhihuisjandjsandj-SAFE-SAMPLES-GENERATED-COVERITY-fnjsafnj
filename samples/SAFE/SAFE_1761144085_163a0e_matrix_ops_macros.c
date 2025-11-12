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
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min_val, max_val);
    }
}

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_SET(matrix, i, j, cols, 
                read_int("Enter matrix element: ", MATRIX_ELEM_MIN, MATRIX_ELEM_MAX));
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
                int term1 = MATRIX_GET(a, i, k, a_cols);
                int term2 = MATRIX_GET(b, k, j, b_cols);
                if ((term1 > 0 && term2 > 0 && term1 > INT_MAX / term2) ||
                    (term1 > 0 && term2 < 0 && term2 < INT_MIN / term1) ||
                    (term1 < 0 && term2 > 0 && term1 < INT_MIN / term2) ||
                    (term1 < 0 && term2 < 0 && term1 < INT_MAX / term2)) {
                    sum = 0;
                    break;
                }
                int product = term1 * term2;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    sum = 0;
                    break;
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
    
    rows1 = read_int("Enter rows for matrix 1: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    
    int matrix1[MATRIX_MAX_SIZE * MATRIX_MAX_SIZE];
    fill_matrix(matrix1, rows1, cols1);
    
    printf("Matrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    rows2 = read_int("Enter rows for matrix 2: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    
    int matrix2[MATRIX_MAX_SIZE * MATRIX_MAX_SIZE];
    fill_matrix(matrix2, rows2, cols2);
    
    printf("Matrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    int result[MATRIX_MAX_SIZE * MATRIX_MAX_SIZE] = {0};
    multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, result);
    
    printf("Result of multiplication:\n");
    print_matrix(result, rows1, cols2);
    
    return 0;
}