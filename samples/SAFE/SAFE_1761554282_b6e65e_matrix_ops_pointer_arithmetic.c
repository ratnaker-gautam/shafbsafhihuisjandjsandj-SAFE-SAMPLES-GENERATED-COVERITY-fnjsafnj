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

int main(void) {
    int rows, cols;
    int matrix_a[MAX_SIZE * MAX_SIZE];
    int matrix_b[MAX_SIZE * MAX_SIZE];
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    
    printf("\nFilling matrix A (%dx%d):\n", rows, cols);
    fill_matrix(matrix_a, rows, cols);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows, cols);
    
    printf("\nTransposing matrix A...\n");
    transpose_matrix(matrix_a, matrix_b, rows, cols);
    
    printf("\nTransposed matrix (%dx%d):\n", cols, rows);
    print_matrix(matrix_b, cols, rows);
    
    printf("\nPerforming element-wise addition...\n");
    if (rows == cols) {
        int result[MAX_SIZE * MAX_SIZE];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int idx = i * cols + j;
                int sum = *(matrix_a + idx) + *(matrix_b + i * cols + j);
                if (sum > 1000000 || sum < -1000000) {
                    printf("Warning: Potential integer overflow detected.\n");
                }
                *(result + idx) = sum;
            }
        }
        printf("Result of A + A_transposed:\n");
        print_matrix(result, rows, cols);
    } else {
        printf("Cannot add matrices of different dimensions.\n");
    }
    
    printf("\nProgram completed.\n");
    return 0;
}