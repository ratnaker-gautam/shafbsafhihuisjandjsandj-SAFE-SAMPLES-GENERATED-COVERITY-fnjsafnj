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
            return -1;
        }
        
        char *endptr;
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
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
    printf("=========================================\n");
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    if (rows == -1) return 1;
    
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    if (cols == -1) return 1;
    
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    printf("\nEnter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                return 1;
            }
            
            char *endptr;
            int value = strtol(buffer, &endptr, 10);
            if (endptr == buffer || *endptr != '\n') {
                printf("Invalid input. Using 0.\n");
                value = 0;
            }
            
            *(*(matrix + i) + j) = value;
        }
    }
    
    printf("\nOriginal Matrix (%dx%d):\n", rows, cols);
    print_matrix((int *)matrix, rows, cols);
    
    if (rows <= MAX_SIZE && cols <= MAX_SIZE) {
        transpose_matrix((int *)matrix, (int *)transposed, rows, cols);
        printf("\nTransposed Matrix (%dx%d):\n", cols, rows);
        print_matrix((int *)transposed, cols, rows);
    }
    
    int row_sum[MAX_SIZE] = {0};
    int col_sum[MAX_SIZE] = {0};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(row_sum + i) += *(*(matrix + i) + j);
            *(col_sum + j) += *(*(matrix + i) + j);
        }
    }
    
    printf("\nRow sums:\n");
    for (int i = 0; i < rows; i++) {
        printf("Row %d: %d\n", i, *(row_sum + i));
    }
    
    printf("\nColumn sums:\n");
    for (int j = 0; j < cols; j++) {
        printf("Col %d: %d\n", j, *(col_sum + j));
    }
    
    return 0;
}