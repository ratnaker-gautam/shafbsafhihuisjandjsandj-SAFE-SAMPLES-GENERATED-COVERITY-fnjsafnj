//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
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

void print_matrix(int* matrix, int rows, int cols) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            *(result + i * b_cols + j) = 0;
            for (int k = 0; k < a_cols; k++) {
                int val1 = *(a + i * a_cols + k);
                int val2 = *(b + k * b_cols + j);
                if ((val1 > 0 && val2 > 0 && val1 > INT_MAX / val2) ||
                    (val1 > 0 && val2 < 0 && val2 < INT_MIN / val1) ||
                    (val1 < 0 && val2 > 0 && val1 < INT_MIN / val2) ||
                    (val1 < 0 && val2 < 0 && val1 < INT_MAX / val2)) {
                    printf("Integer overflow detected in multiplication.\n");
                    *(result + i * b_cols + j) = 0;
                    break;
                }
                int product = val1 * val2;
                if ((val1 > 0 && val2 > 0 && product < 0) ||
                    (val1 < 0 && val2 < 0 && product < 0) ||
                    (val1 > 0 && val2 < 0 && product > 0) ||
                    (val1 < 0 && val2 > 0 && product > 0)) {
                    printf("Integer overflow detected in multiplication.\n");
                    *(result + i * b_cols + j) = 0;
                    break;
                }
                int sum = *(result + i * b_cols + j) + product;
                if ((*(result + i * b_cols + j) > 0 && product > 0 && sum < 0) ||
                    (*(result + i * b_cols + j) < 0 && product < 0 && sum > 0)) {
                    printf("Integer overflow detected in addition.\n");
                    *(result + i * b_cols + j) = 0;
                    break;
                }
                *(result + i * b_cols + j) = sum;
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("==============================================\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix (%d) must equal rows of second matrix (%d).\n", cols1, rows2);
        return 1;
    }
    
    size_t size1 = (size_t)rows1 * (size_t)cols1;
    size_t size2 = (size_t)rows2 * (size_t)cols2;
    size_t size_result = (size_t)rows1 * (size_t)cols2;
    
    if (size1 > SIZE_MAX / sizeof(int) || size2 > SIZE_MAX / sizeof(int) || 
        size_result > SIZE_MAX / sizeof(int)) {
        printf("Error: Matrix dimensions too large.\n");
        return 1;
    }
    
    int* matrix1 = malloc(size1 * sizeof(int));
    int* matrix2 = malloc(size2 * sizeof(int));
    int* result = malloc(size_result * sizeof(int));
    
    if (matrix1 == NULL