//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) {
                printf("Input error.\n");
                exit(1);
            }
            *(ptr + i * mat->cols + j) = val;
        }
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a_ptr + i * a->cols + k);
                int b_val = *(b_ptr + k * b->cols + j);
                if ((a_val > 0 && b_val > 0 && a_val > INT_MAX / b_val) ||
                    (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                    (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val) ||
                    (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val)) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                int product = a_val * b_val;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                sum += product;
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    mat1.rows = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    if (mat1.rows == -1) return 1;
    
    mat1.cols = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    if (mat1.cols == -1) return 1;
    
    printf("\n");
    read_matrix(&mat1);
    
    printf("\nMatrix 2 must have %d rows for multiplication.\n", mat1.cols);
    mat2.rows = mat1.cols;
    
    mat2.cols = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    if (mat2.cols == -1) return 1;
    
    printf("\n");
    read_matrix(&mat2);
    
    if (!validate_dimension(mat1.rows) || !validate_dimension(mat1.cols) ||
        !validate_dimension(mat2.rows) || !validate_dimension(mat2.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    result.rows = mat1.rows;
    result.cols = mat2.cols;
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nResult of multiplication:\n");
    multiply_matrices(&mat1, &mat2, &result);
    print_matrix(&result);
    
    return 0;
}