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
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements row by row:\n");
    
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *ptr = read_int(prompt, INT_MIN, INT_MAX);
            ptr++;
        }
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *ptr);
            ptr++;
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* ptr_a = a->data;
    const int* ptr_b = b->data;
    int* ptr_result = result->data;
    
    int size = a->rows * a->cols;
    for (int i = 0; i < size; i++) {
        if ((*ptr_b > 0 && *ptr_a > INT_MAX - *ptr_b) ||
            (*ptr_b < 0 && *ptr_a < INT_MIN - *ptr_b)) {
            printf("Integer overflow detected in addition.\n");
            exit(1);
        }
        *ptr_result = *ptr_a + *ptr_b;
        ptr_a++;
        ptr_b++;
        ptr_result++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    int* ptr_result = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                const int* elem_a = a->data + (i * a->cols + k);
                const int* elem_b = b->data + (k * b->cols + j);
                
                long product = (long)(*elem_a) * (long)(*elem_b);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    exit(1);
                }
                
                if ((sum > 0 && (int)product > INT_MAX - sum) ||
                    (sum < 0 && (int)product < INT_MIN - sum)) {
                    printf("Integer overflow detected in sum.\n");
                    exit(1);
                }
                
                sum += (int)product;
            }
            *ptr_result = sum;
            ptr_result++;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.rows == -1) return 1;
    
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.cols == -1) return 1;
    
    read_matrix(&mat1);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    mat2.rows = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    if (mat2.rows == -1) return 1;
    
    mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    if (mat2.cols == -1) return 1;
    
    read_matrix(&mat2);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nChoose operation:\n");
    printf("1. Matrix Addition\n");
    printf("2. Matrix Multiplication\n");
    
    int choice = read_int("Enter choice (1-2): ", 1, 2);
    if (choice ==