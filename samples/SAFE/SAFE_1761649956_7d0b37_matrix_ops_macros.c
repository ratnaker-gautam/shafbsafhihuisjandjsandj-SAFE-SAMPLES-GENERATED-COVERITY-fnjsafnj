//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return value;
    }
}

void read_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), 
                    "Enter element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix, i, j, cols) = 
                read_int(prompt, INT_MIN / MAX_ROWS, INT_MAX / MAX_ROWS);
        }
    }
}

void print_matrix(const int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(const int* a, const int* b, int* result, 
                      int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                int term1 = MATRIX_ELEM(a, i, k, a_cols);
                int term2 = MATRIX_ELEM(b, k, j, b_cols);
                if ((term1 > 0 && term2 > 0 && term1 > INT_MAX / term2) ||
                    (term1 > 0 && term2 < 0 && term2 < INT_MIN / term1) ||
                    (term1 < 0 && term2 > 0 && term1 < INT_MIN / term2) ||
                    (term1 < 0 && term2 < 0 && term1 < INT_MAX / term2)) {
                    sum = 0;
                    break;
                }
                int product = term1 * term2;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    sum = 0;
                    break;
                }
                sum += product;
            }
            MATRIX_ELEM(result, i, j, b_cols) = sum;
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("First matrix dimensions:\n");
    rows1 = read_int("Enter rows: ", MIN_DIM, MAX_ROWS);
    cols1 = read_int("Enter columns: ", MIN_DIM, MAX_COLS);
    
    printf("Second matrix dimensions:\n");
    rows2 = read_int("Enter rows: ", MIN_DIM, MAX_ROWS);
    cols2 = read_int("Enter columns: ", MIN_DIM, MAX_COLS);
    
    if (cols1 != rows2) {
        printf("Error: Cannot multiply matrices. Columns of first must equal rows of second.\n");
        return 1;
    }
    
    int matrix1[MAX_ROWS * MAX_COLS];
    int matrix2[MAX_ROWS * MAX_COLS];
    int result[MAX_ROWS * MAX_COLS];
    
    memset(matrix1, 0, sizeof(matrix1));
    memset(matrix2, 0, sizeof(matrix2));
    memset(result, 0, sizeof(result));
    
    printf("\nEnter first matrix elements:\n");
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter second matrix elements:\n");
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nFirst matrix:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nSecond matrix:\n");
    print_matrix(matrix2, rows2, cols2);
    
    multiply_matrices(matrix1, matrix2, result, rows1, cols1, cols2);
    
    printf("\nResult matrix:\n");
    print_matrix(result, rows1, cols2);
    
    return 0;
}