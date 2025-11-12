//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_IDX(mat, row, col, cols) ((mat)[(row) * (cols) + (col)])

void print_matrix(int *mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", MATRIX_IDX(mat, i, j, cols));
        }
        printf("\n");
    }
}

int read_int_safe(int min, int max) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (val < min || val > max) {
        return -1;
    }
    return (int)val;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter first matrix rows (1-%d): ", MAX_DIM);
    rows1 = read_int_safe(1, MAX_DIM);
    if (rows1 == -1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter first matrix columns (1-%d): ", MAX_DIM);
    cols1 = read_int_safe(1, MAX_DIM);
    if (cols1 == -1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter second matrix rows (1-%d): ", MAX_DIM);
    rows2 = read_int_safe(1, MAX_DIM);
    if (rows2 == -1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter second matrix columns (1-%d): ", MAX_DIM);
    cols2 = read_int_safe(1, MAX_DIM);
    if (cols2 == -1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    int mat1[rows1 * cols1];
    int mat2[rows2 * cols2];
    int result[rows1 * cols2];
    
    printf("Enter first matrix elements:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int_safe(INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid input\n");
                return 1;
            }
            MATRIX_IDX(mat1, i, j, cols1) = val;
        }
    }
    
    printf("Enter second matrix elements:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int_safe(INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid input\n");
                return 1;
            }
            MATRIX_IDX(mat2, i, j, cols2) = val;
        }
    }
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            int sum = 0;
            for (int k = 0; k < cols1; k++) {
                long long product = (long long)MATRIX_IDX(mat1, i, k, cols1) * 
                                   (long long)MATRIX_IDX(mat2, k, j, cols2);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected\n");
                    return 1;
                }
                long long new_sum = (long long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    printf("Integer overflow detected\n");
                    return 1;
                }
                sum = (int)new_sum;
            }
            MATRIX_IDX(result, i, j, cols2) = sum;
        }
    }
    
    printf("First matrix:\n");
    print_matrix(mat1, rows1, cols1);
    
    printf("Second matrix:\n");
    print_matrix(mat2, rows2, cols2);
    
    printf("Result matrix:\n");
    print_matrix(result, rows1, cols2);
    
    return 0;
}