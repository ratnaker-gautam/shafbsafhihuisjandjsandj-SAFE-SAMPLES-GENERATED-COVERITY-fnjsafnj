//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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
        
        if (temp < min || temp > max || temp > INT_MAX) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements row by row:\n");
    
    int* ptr = mat->data;
    int total = mat->rows * mat->cols;
    
    for (int i = 0; i < total; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Element [%d,%d]: ", 
                i / mat->cols, i % mat->cols);
        
        int value = read_int(prompt, -1000, 1000);
        if (value == -1) {
            printf("Input error.\n");
            exit(1);
        }
        
        *(ptr + i) = value;
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        exit(1);
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    int* res_ptr = result->data;
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row = a_ptr + i * a->cols;
            const int* b_col = b_ptr + j;
            
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)*(a_row + k) * (long long)*(b_col + k * b->cols);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                if (sum > 0 && product > INT_MAX - sum) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                if (sum < 0 && product < INT_MIN - sum) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                sum += (int)product;
            }
            
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("Maximum dimension: %d\n", MAX_DIM);
    
    mat1.rows = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    
    if (!validate_dimension(mat1.rows) || !validate_dimension(mat1.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    printf("Matrix 1: %d x %d\n", mat1.rows, mat1.cols);
    read_matrix(&mat1);
    
    mat2.rows = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    if (!validate_dimension(mat2.rows) || !validate_dimension(mat2.cols)) {
        printf("Invalid matrix dimensions.\n");
        return 1;
    }
    
    printf("Matrix 2: %d x %d\n", mat2.rows, mat2.cols);
    read_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    multiply_matrices(&mat1, &mat2, &result);
    
    printf("\nResult of multiplication:\n");
    print_matrix(&result);
    
    return 0;
}