//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return min;
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

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void transpose_matrix(int* src, int* dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int main() {
    int rows, cols;
    
    printf("Matrix Transpose Operation\n");
    printf("==========================\n");
    
    rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    
    if (rows > MAX_DIM || cols > MAX_DIM) {
        printf("Matrix dimensions too large.\n");
        return 1;
    }
    
    size_t matrix_size = (size_t)rows * cols;
    if (matrix_size > SIZE_MAX / sizeof(int)) {
        printf("Matrix too large for memory.\n");
        return 1;
    }
    
    int* matrix = malloc(matrix_size * sizeof(int));
    if (matrix == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    printf("\nFilling matrix %dx%d:\n", rows, cols);
    fill_matrix(matrix, rows, cols);
    
    printf("\nOriginal matrix:\n");
    print_matrix(matrix, rows, cols);
    
    size_t transpose_size = (size_t)cols * rows;
    if (transpose_size > SIZE_MAX / sizeof(int)) {
        printf("Transpose matrix too large for memory.\n");
        free(matrix);
        return 1;
    }
    
    int* transpose = malloc(transpose_size * sizeof(int));
    if (transpose == NULL) {
        printf("Memory allocation failed.\n");
        free(matrix);
        return 1;
    }
    
    transpose_matrix(matrix, transpose, rows, cols);
    
    printf("\nTransposed matrix (%dx%d):\n", cols, rows);
    print_matrix(transpose, cols, rows);
    
    free(matrix);
    free(transpose);
    
    return 0;
}