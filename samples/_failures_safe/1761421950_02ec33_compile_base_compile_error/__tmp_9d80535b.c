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

void init_matrix(Matrix* mat) {
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
        if ((*(ptr_a + i) > 0 && *(ptr_b + i) > INT_MAX - *(ptr_a + i)) ||
            (*(ptr_a + i) < 0 && *(ptr_b + i) < INT_MIN - *(ptr_a + i))) {
            printf("Integer overflow detected!\n");
            exit(1);
        }
        *(ptr_result + i) = *(ptr_a + i) + *(ptr_b + i);
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* ptr_a = a->data;
    const int* ptr_b = b->data;
    int* ptr_result = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)*(ptr_a + i * a->cols + k) * (long)*(ptr_b + k * b->cols + j);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
                if ((sum > 0 && product > INT_MAX - sum) || (sum < 0 && product < INT_MIN - sum)) {
                    printf("Integer overflow detected!\n");
                    exit(1);
                }
                sum += (int)product;
            }
            *(ptr_result + i * result->cols + j) = sum;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    
    printf("\nMatrix 1:\n");
    init_matrix(&mat1);
    
    mat2.rows = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    
    printf("\nMatrix 2:\n");
    init_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    int choice;
    do {
        printf("\nOperations:\n");
        printf("1. Add matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Exit\n");
        
        choice = read_int("Enter your choice (1-3): ", 1, 3);
        
        switch (choice) {
            case 1:
                if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
                    printf("Error: Matrices must have same dimensions