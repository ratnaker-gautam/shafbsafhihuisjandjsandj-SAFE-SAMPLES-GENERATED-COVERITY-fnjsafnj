//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
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

void fill_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(matrix + i * cols + j) = read_int("Enter element: ", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void add_matrices(int *a, int *b, int *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            int sum = *(a + idx) + *(b + idx);
            if ((*(a + idx) > 0 && *(b + idx) > 0 && sum < 0) ||
                (*(a + idx) < 0 && *(b + idx) < 0 && sum > 0)) {
                printf("Warning: Integer overflow detected at position (%d,%d)\n", i, j);
            }
            *(result + idx) = sum;
        }
    }
}

void multiply_matrices(int *a, int *b, int *result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                int a_val = *(a + i * a_cols + k);
                int b_val = *(b + k * b_cols + j);
                long long product = (long long)a_val * (long long)b_val;
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Warning: Multiplication overflow at (%d,%d,%d)\n", i, j, k);
                }
                sum += (int)product;
                if ((product > 0 && sum < 0) || (product < 0 && sum > 0)) {
                    printf("Warning: Addition overflow at (%d,%d,%d)\n", i, j, k);
                }
            }
            *(result + i * b_cols + j) = sum;
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    
    int matrix1[MAX_DIM * MAX_DIM];
    printf("Filling matrix 1:\n");
    fill_matrix(matrix1, rows1, cols1);
    
    rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    int matrix2[MAX_DIM * MAX_DIM];
    printf("Filling matrix 2:\n");
    fill_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    if (rows1 == rows2 && cols1 == cols2) {
        int result_add[MAX_DIM * MAX_DIM];
        add_matrices(matrix1, matrix2, result_add, rows1, cols1);
        printf("\nMatrix Addition Result:\n");
        print_matrix(result_add, rows1, cols1);
    } else {
        printf("\nMatrices cannot be added (dimensions differ)\n");
    }
    
    if (cols1 == rows2) {
        int result_mult[MAX_DIM * MAX_DIM];
        multiply_matrices(matrix1, matrix2, result_mult, rows1, cols1, cols2);
        printf("\nMatrix Multiplication Result:\n");
        print_matrix(result_mult, rows1, cols2);
    } else {
        printf("\nMatrices cannot be multiplied (columns of first != rows of second)\n");
    }
    
    return 0;
}