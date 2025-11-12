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
    int total = mat->rows * mat->cols;
    
    for (int i = 0; i < total; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Element [%d]: ", i);
        *(ptr + i) = read_int(prompt, INT_MIN, INT_MAX);
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
    const int* ptr_a = a->data;
    const int* ptr_b = b->data;
    int* ptr_result = result->data;
    int total = a->rows * a->cols;
    
    for (int i = 0; i < total; i++) {
        int sum = *(ptr_a + i) + *(ptr_b + i);
        if ((*(ptr_a + i) > 0 && *(ptr_b + i) > 0 && sum < 0) ||
            (*(ptr_a + i) < 0 && *(ptr_b + i) < 0 && sum > 0)) {
            printf("Integer overflow detected!\n");
            exit(1);
        }
        *(ptr_result + i) = sum;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* ptr_a = a->data;
    const int* ptr_b = b->data;
    int* ptr_result = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)*(ptr_a + i * a->cols + k) * 
                              (long)*(ptr_b + k * b->cols + j);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
            }
            *(ptr_result + i * result->cols + j) = (int)sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    read_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    read_matrix(&mat2);
    
    printf("\nFirst Matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
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
            result.rows = mat1.rows;
            result.cols = mat1.cols;
            add_matrices(&mat