//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

int validate_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

void read_matrix(int *matrix, int rows, int cols) {
    printf("Enter %d elements for %dx%d matrix:\n", rows * cols, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int value;
            if (scanf("%d", &value) != 1) {
                printf("Invalid input\n");
                exit(EXIT_FAILURE);
            }
            *(matrix + i * cols + j) = value;
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void transpose_matrix(int *src, int *dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int main(void) {
    int rows, cols;
    
    printf("Enter matrix dimensions (rows columns, max %dx%d): ", MAX_SIZE, MAX_SIZE);
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("Invalid dimensions\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_dimensions(rows, cols)) {
        printf("Invalid dimensions\n");
        return EXIT_FAILURE;
    }
    
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    read_matrix((int *)matrix, rows, cols);
    
    printf("\nOriginal matrix:\n");
    print_matrix((int *)matrix, rows, cols);
    
    transpose_matrix((int *)matrix, (int *)transposed, rows, cols);
    
    printf("\nTransposed matrix:\n");
    print_matrix((int *)transposed, cols, rows);
    
    return EXIT_SUCCESS;
}