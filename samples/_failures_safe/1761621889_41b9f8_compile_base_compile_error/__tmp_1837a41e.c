//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_IDX(mat, row, col, cols) ((mat)[(row) * (cols) + (col)])

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", MATRIX_IDX(matrix, i, j, cols));
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_DIM);
    
    rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    if (rows1 == -1) return 1;
    cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    if (cols1 == -1) return 1;
    
    rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    if (rows2 == -1) return 1;
    cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    if (cols2 == -1) return 1;
    
    if (rows1 != rows2 || cols1 != cols2) {
        printf("Matrices must have same dimensions for addition.\n");
        return 1;
    }
    
    int size1 = rows1 * cols1;
    int size2 = rows2 * cols2;
    
    if (size1 > INT_MAX / sizeof(int) || size2 > INT_MAX / sizeof(int)) {
        printf("Matrix size too large.\n");
        return 1;
    }
    
    int *matrix1 = malloc(size1 * sizeof(int));
    int *matrix2 = malloc(size2 * sizeof(int));
    int *result = malloc(size1 * sizeof(int));
    
    if (!matrix1 || !matrix2 || !result) {
        printf("Memory allocation failed.\n");
        free(matrix1);
        free(matrix2);
        free(result);
        return 1;
    }
    
    printf("Enter elements for matrix 1 (%d x %d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_IDX(matrix1, i, j, cols1) = read_int(prompt, INT_MIN, INT_MAX);
            if (MATRIX_IDX(matrix1, i, j, cols1) == -1) {
                free(matrix1);
                free(matrix2);
                free(result);
                return 1;
            }
        }
    }
    
    printf("Enter elements for matrix 2 (%d x %d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_IDX(matrix2, i, j, cols2) = read_int(prompt, INT_MIN, INT_MAX);
            if (MATRIX_IDX(matrix2, i, j, cols2) == -1) {
                free(matrix1);
                free(matrix2);
                free(result);
                return 1;
            }
        }
    }
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix Addition Result:\n");
    int overflow_detected = 0;
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            int val1 = MATRIX_IDX(matrix1, i, j, cols1);
            int val2 = MATRIX_IDX(matrix2, i, j, cols2);
            int sum = val1 + val2;
            
            if ((val1 > 0 && val2 > 0 && sum < 0) ||
                (val1 < 0 && val2 < 0 && sum > 0)) {
                printf("Overflow detected at [%d][%d]\n", i, j);
                overflow_det