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
    char buffer[100];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
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
    printf("Enter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(matrix + i * cols + j) = read_int(prompt, -1000, 1000);
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
    printf("=========================================\n");
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    if (rows == -1) return 1;
    
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    if (cols == -1) return 1;
    
    int matrix[MAX_SIZE * MAX_SIZE];
    int transposed[MAX_SIZE * MAX_SIZE];
    
    fill_matrix(matrix, rows, cols);
    
    printf("\nOriginal Matrix:\n");
    print_matrix(matrix, rows, cols);
    
    memset(transposed, 0, sizeof(transposed));
    transpose_matrix(matrix, transposed, rows, cols);
    
    printf("\nTransposed Matrix:\n");
    print_matrix(transposed, cols, rows);
    
    printf("\nMatrix Statistics:\n");
    int sum = 0;
    int min = *(matrix);
    int max = *(matrix);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val = *(matrix + i * cols + j);
            sum += val;
            if (val < min) min = val;
            if (val > max) max = val;
        }
    }
    
    printf("Sum: %d\n", sum);
    printf("Average: %.2f\n", (double)sum / (rows * cols));
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
    
    return 0;
}