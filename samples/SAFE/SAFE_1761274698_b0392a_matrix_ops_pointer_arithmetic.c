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
            printf("Input error\n");
            exit(1);
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

void transpose_matrix(int *src, int *dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(dest + j * rows + i) = *(src + i * cols + j);
        }
    }
}

int main() {
    int rows, cols;
    int matrix[MAX_SIZE][MAX_SIZE];
    int transposed[MAX_SIZE][MAX_SIZE];
    
    printf("Matrix Transposition Program\n");
    printf("============================\n\n");
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    printf("\nEnter matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            char buffer[100];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                printf("Input error\n");
                return 1;
            }
            
            int value;
            if (sscanf(buffer, "%d", &value) != 1) {
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
    
    printf("\nOperation completed successfully.\n");
    return 0;
}