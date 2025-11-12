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

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements row by row:\n");
    
    int* ptr = mat->data;
    int total = mat->rows * mat->cols;
    
    for (int i = 0; i < total; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Element [%d]: ", i);
        int value = read_int(prompt, -1000, 1000);
        if (value == -1) {
            exit(EXIT_FAILURE);
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
                    (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val) ||
                    (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                    (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val)) {
                    printf("Integer overflow detected in multiplication.\n");
                    exit(EXIT_FAILURE);
                }
                
                int product = a_val * b_val;
                
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    printf("Integer overflow detected in summation.\n");
                    exit(EXIT_FAILURE);
                }
                
                sum += product;
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum dimension: %d\n\n", MAX_DIM);
    
    mat1.rows = read_int("Enter rows for first matrix: ", 1, MAX_DIM);
    if (mat1.rows == -1) return EXIT_FAILURE;
    
    mat1.cols = read_int("Enter columns for first matrix: ", 1, MAX_DIM);
    if (mat1.cols == -1) return EXIT_FAILURE;
    
    printf("\nFirst matrix dimensions: %d x %d\n", mat1.rows, mat1.cols);
    read_matrix(&mat1);
    
    printf("\nSecond matrix must have %d rows\n", mat1.cols);
    mat2.rows = mat1.cols;
    mat2.cols = read_int("Enter columns for second matrix: ", 1, MAX_DIM);
    if (mat2.cols == -1) return EXIT_FAILURE;
    
    printf("\nSecond matrix dimensions: %d x %d\n", mat2.rows, mat2.cols);
    read_matrix(&mat2);
    
    result.rows = mat1.rows;
    result.cols = mat2.cols;
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMultiplying matrices...\n");
    multiply_matrices(&mat1, &mat2, &result);