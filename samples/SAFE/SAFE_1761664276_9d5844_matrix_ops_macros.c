//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

int read_int_safe(int *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *value = (int)val;
    return 1;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Enter first matrix rows (1-%d): ", MAX_DIM);
    if (!read_int_safe(&rows1) || rows1 < 1 || rows1 > MAX_DIM) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter first matrix columns (1-%d): ", MAX_DIM);
    if (!read_int_safe(&cols1) || cols1 < 1 || cols1 > MAX_DIM) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter second matrix rows (1-%d): ", MAX_DIM);
    if (!read_int_safe(&rows2) || rows2 < 1 || rows2 > MAX_DIM) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter second matrix columns (1-%d): ", MAX_DIM);
    if (!read_int_safe(&cols2) || cols2 < 1 || cols2 > MAX_DIM) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return 1;
    }
    
    int mat1[MAX_DIM * MAX_DIM];
    int mat2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Enter first matrix elements:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (!read_int_safe(&MATRIX_ELEM(mat1, i, j, cols1))) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    printf("Enter second matrix elements:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (!read_int_safe(&MATRIX_ELEM(mat2, i, j, cols2))) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    memset(result, 0, sizeof(result));
    
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            long sum = 0;
            for (int k = 0; k < cols1; k++) {
                long prod = (long)MATRIX_ELEM(mat1, i, k, cols1) * 
                           (long)MATRIX_ELEM(mat2, k, j, cols2);
                if (prod > INT_MAX - sum || prod < INT_MIN + sum) {
                    printf("Integer overflow detected\n");
                    return 1;
                }
                sum += prod;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Integer overflow detected\n");
                return 1;
            }
            MATRIX_ELEM(result, i, j, cols2) = (int)sum;
        }
    }
    
    printf("Result matrix:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("%d ", MATRIX_ELEM(result, i, j, cols2));
        }
        printf("\n");
    }
    
    return 0;
}