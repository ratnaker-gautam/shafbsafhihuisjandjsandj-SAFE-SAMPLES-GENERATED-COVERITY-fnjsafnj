//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(matrix, i, j, cols) (matrix)[(i) * (cols) + (j)]

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }
}

int read_int_safe(const char *prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char *endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Matrix Operations Program\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_DIM);
    
    rows1 = read_int_safe("Enter rows for matrix 1: ", 1, MAX_DIM);
    cols1 = read_int_safe("Enter columns for matrix 1: ", 1, MAX_DIM);
    
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix1, i, j, cols1) = read_int_safe(prompt, INT_MIN, INT_MAX);
        }
    }
    
    rows2 = read_int_safe("Enter rows for matrix 2: ", 1, MAX_DIM);
    cols2 = read_int_safe("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    printf("Enter elements for matrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix2, i, j, cols2) = read_int_safe(prompt, INT_MIN, INT_MAX);
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    if (rows1 == rows2 && cols1 == cols2) {
        printf("\nMatrix Addition:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                MATRIX_ELEM(result, i, j, cols1) = MATRIX_ELEM(matrix1, i, j, cols1) + MATRIX_ELEM(matrix2, i, j, cols1);
            }
        }
        print_matrix(result, rows1, cols1);
    } else {
        printf("\nMatrices have different dimensions, cannot perform addition.\n");
    }
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                int sum = 0;
                for (int k = 0; k < cols1; k++) {
                    sum += MATRIX_ELEM(matrix1, i, k, cols1) * MATRIX_ELEM(matrix2, k, j, cols2);
                }
                MATRIX_ELEM(result, i, j, cols2) = sum;
            }
        }
        print_matrix(result, rows1, cols2);
    } else {
        printf("\nMatrices cannot be multiplied (columns of matrix 1 must equal rows of matrix 2).\n");
    }
    
    printf("\nMatrix 1 Transpose:\n");
    for (int i = 0; i < cols1; i++) {
        for (int j = 0; j < rows1; j++) {
            MATRIX_ELEM(result, i, j, rows1) = MATRIX_ELEM(matrix1, j, i, cols1);
        }
    }
    print_matrix(result, cols1, rows1);
    
    return 0;
}