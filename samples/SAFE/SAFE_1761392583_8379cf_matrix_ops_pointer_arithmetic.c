//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 16

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        char* endptr;
        long lval = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (lval < min || lval > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)lval;
        break;
    }
    
    return value;
}

void matrix_multiply(int* a, int* b, int* result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            *(result + i * cols_b + j) = 0;
            for (int k = 0; k < cols_a; k++) {
                int val_a = *(a + i * cols_a + k);
                int val_b = *(b + k * cols_b + j);
                if ((val_a > 0 && val_b > INT_MAX / val_a) || 
                    (val_a < 0 && val_b < INT_MIN / val_a)) {
                    *(result + i * cols_b + j) = 0;
                    break;
                }
                *(result + i * cols_b + j) += val_a * val_b;
            }
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", *(matrix + i * cols + j));
        }
        printf("\n");
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

int main(void) {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix A dimensions:\n");
    rows_a = read_int("Rows (1-16): ", 1, MAX_DIM);
    cols_a = read_int("Columns (1-16): ", 1, MAX_DIM);
    
    printf("Matrix B dimensions:\n");
    rows_b = read_int("Rows (1-16): ", 1, MAX_DIM);
    cols_b = read_int("Columns (1-16): ", 1, MAX_DIM);
    
    if (cols_a != rows_b) {
        printf("Error: Matrix A columns must equal Matrix B rows for multiplication.\n");
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("\nEnter Matrix A elements:\n");
    fill_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nEnter Matrix B elements:\n");
    fill_matrix(matrix_b, rows_b, cols_b);
    
    matrix_multiply(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nResult (A × B):\n");
    print_matrix(result, rows_a, cols_b);
    
    return 0;
}