//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

int read_int_safe(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Enter value between %d and %d.\n", min_val, max_val);
    }
}

void fill_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_ELEM(mat, i, j, cols) = read_int_safe("Enter element: ", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(mat, i, j, cols));
        }
        printf("\n");
    }
}

void transpose_matrix(const int* src, int* dst, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_ELEM(dst, j, i, rows) = MATRIX_ELEM(src, i, j, cols);
        }
    }
}

int multiply_matrices(const int* a, int a_rows, int a_cols, const int* b, int b_rows, int b_cols, int* result) {
    if (a_cols != b_rows) {
        return 0;
    }
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += MATRIX_ELEM(a, i, k, a_cols) * MATRIX_ELEM(b, k, j, b_cols);
            }
            MATRIX_ELEM(result, i, j, b_cols) = sum;
        }
    }
    return 1;
}

int main() {
    int choice;
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Matrix Operations Program\n");
    
    while (1) {
        printf("\n1. Transpose matrix\n");
        printf("2. Multiply matrices\n");
        printf("3. Exit\n");
        choice = read_int_safe("Select operation: ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        if (choice == 1) {
            rows1 = read_int_safe("Enter rows: ", 1, MAX_DIM);
            cols1 = read_int_safe("Enter columns: ", 1, MAX_DIM);
            printf("Enter matrix elements:\n");
            fill_matrix(matrix1, rows1, cols1);
            printf("Original matrix:\n");
            print_matrix(matrix1, rows1, cols1);
            transpose_matrix(matrix1, result, rows1, cols1);
            printf("Transposed matrix:\n");
            print_matrix(result, cols1, rows1);
        }
        else if (choice == 2) {
            rows1 = read_int_safe("Enter rows for matrix 1: ", 1, MAX_DIM);
            cols1 = read_int_safe("Enter columns for matrix 1: ", 1, MAX_DIM);
            rows2 = read_int_safe("Enter rows for matrix 2: ", 1, MAX_DIM);
            cols2 = read_int_safe("Enter columns for matrix 2: ", 1, MAX_DIM);
            
            if (cols1 != rows2) {
                printf("Matrix dimensions incompatible for multiplication.\n");
                continue;
            }
            
            printf("Enter matrix 1 elements:\n");
            fill_matrix(matrix1, rows1, cols1);
            printf("Enter matrix 2 elements:\n");
            fill_matrix(matrix2, rows2, cols2);
            
            printf("Matrix 1:\n");
            print_matrix(matrix1, rows1, cols1);
            printf("Matrix 2:\n");
            print_matrix(matrix2, rows2, cols2);
            
            if (multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, result)) {
                printf("Product matrix:\n");
                print_matrix(result, rows1, cols2);
            } else {
                printf("Multiplication failed.\n");
            }
        }
    }
    
    return 0;
}