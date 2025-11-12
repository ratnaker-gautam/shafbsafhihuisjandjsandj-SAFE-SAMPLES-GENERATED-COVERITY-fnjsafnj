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
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        if (temp > INT_MAX || temp < INT_MIN) {
            printf("Value out of range.\n");
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void init_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    int total = mat->rows * mat->cols;
    
    for (int i = 0; i < total; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Element [%d]: ", i);
        *(ptr + i) = read_int(prompt, -1000, 1000);
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

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
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
                int a_val = *(a_row + k);
                int b_val = *(b_col + k * b->cols);
                
                if ((a_val > 0 && b_val > 0 && a_val > INT_MAX / b_val) ||
                    (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                    (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val) ||
                    (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val)) {
                    return -1;
                }
                
                int product = a_val * b_val;
                
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return -1;
                }
                
                sum += product;
            }
            
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("==============================================\n\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\nFirst Matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    print_matrix(&mat2);
    
    int multiply_result = multiply_matrices(&mat1, &mat2, &result);
    
    if (multiply_result == 0) {
        printf("\nError: Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix (%d) must equal rows of second matrix (%d).\n", mat1.cols, mat2.rows);
        return 1;
    } else if (multiply_result == -1) {
        printf("\nError: Integer overflow occurred during multiplication.\n");
        return 1;
    }
    
    printf("\nResult