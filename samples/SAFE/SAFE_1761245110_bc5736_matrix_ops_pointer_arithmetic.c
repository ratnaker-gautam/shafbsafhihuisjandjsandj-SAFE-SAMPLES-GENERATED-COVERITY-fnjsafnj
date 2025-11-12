//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", *(matrix + i * cols + j));
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
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void fill_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int("", -1000, 1000);
        }
    }
}

void transpose_matrix(int *src, int *dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int main() {
    int rows, cols;
    
    printf("Matrix Operations using Pointer Arithmetic\n");
    printf("==========================================\n\n");
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    printf("\nFilling matrix %dx%d:\n", rows, cols);
    fill_matrix((int *)matrix, rows, cols);
    
    printf("\nOriginal matrix:\n");
    print_matrix((int *)matrix, rows, cols);
    
    printf("\nTransposed matrix:\n");
    transpose_matrix((int *)matrix, (int *)transposed, rows, cols);
    print_matrix((int *)transposed, cols, rows);
    
    printf("\nMatrix operations completed.\n");
    return 0;
}