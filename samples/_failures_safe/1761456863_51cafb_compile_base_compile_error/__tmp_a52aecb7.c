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
            printf("Invalid input. Please enter a valid integer.\n");
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
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
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

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int idx = i * a->cols + j;
            long sum = (long)*(a_ptr + idx) + (long)*(b_ptr + idx);
            
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Overflow detected in addition.\n");
                return;
            }
            
            *(res_ptr + idx) = (int)sum;
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            
            for (int k = 0; k < a->cols; k++) {
                long product = (long)*(a_ptr + i * a->cols + k) * 
                              (long)*(b_ptr + k * b->cols + j);
                
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Overflow detected in multiplication.\n");
                    return;
                }
                
                sum += product;
                
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Overflow detected in summation.\n");
                    return;
                }
            }
            
            *(res_ptr + i * result->cols + j) = (int)sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    printf("\n");
    read_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\n");
    mat2.rows = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    printf("\n");
    read_matrix(&mat2);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nSelect operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = read_int("Enter choice (1-2): ", 1, 2);
    
    switch (choice) {
        case 1:
            if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
                printf("Error: Matrices must have same dimensions for addition.\n");
                return 1;
            }
            result