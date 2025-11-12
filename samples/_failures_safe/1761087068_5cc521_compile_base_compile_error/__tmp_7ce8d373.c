//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

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
    long val;
    char *endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (val < min_val || val > max_val) {
        return -1;
    }
    
    return (int)val;
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    int transpose[MAX_DIM * MAX_DIM];
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int_safe("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    if (rows1 == -1) {
        printf("Invalid input for rows1\n");
        return 1;
    }
    
    cols1 = read_int_safe("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    if (cols1 == -1) {
        printf("Invalid input for cols1\n");
        return 1;
    }
    
    printf("Enter elements for matrix 1:\n");
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            int val = read_int_safe("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid matrix element\n");
                return 1;
            }
            MATRIX_ELEM(matrix1, i, j, cols1) = val;
        }
    }
    
    rows2 = read_int_safe("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    if (rows2 == -1) {
        printf("Invalid input for rows2\n");
        return 1;
    }
    
    cols2 = read_int_safe("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    if (cols2 == -1) {
        printf("Invalid input for cols2\n");
        return 1;
    }
    
    printf("Enter elements for matrix 2:\n");
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            int val = read_int_safe("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Invalid matrix element\n");
                return 1;
            }
            MATRIX_ELEM(matrix2, i, j, cols2) = val;
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
                MATRIX_ELEM(result, i, j, cols1) = 
                    MATRIX_ELEM(matrix1, i, j, cols1) + 
                    MATRIX_ELEM(matrix2, i, j, cols2);
            }
        }
        print_matrix(result, rows1, cols1);
    } else {
        printf("\nMatrices cannot be added (dimensions differ)\n");
    }
    
    if (cols1 == rows2) {
        printf("\nMatrix Multiplication:\n");
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols2; j++) {
                int sum = 0;
                for (int k = 0; k < cols1; k++) {
                    sum += MATRIX_ELEM(matrix1, i, k, cols1) * 
                           MATRIX_ELEM(matrix2, k, j, cols2);
                }
                MATRIX_ELEM(result, i, j, cols2) = sum;
            }
        }
        print_matrix(result, rows1, cols2);
    } else {
        printf("\nMatrices cannot be multiplied (cols1 != rows2)\n");
    }
    
    printf("\nMatrix 1 Transpose:\n");
    for (int i = 0; i < cols1; i++) {
        for (int