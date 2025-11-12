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

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void init_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        printf("Element [%d][%d]: ", i / mat->cols, i % mat->cols);
        int val = read_int("", INT_MIN, INT_MAX);
        if (val == -1) {
            exit(1);
        }
        *(mat->data + i) = val;
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions don't match for addition.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    int size = a->rows * a->cols;
    for (int i = 0; i < size; i++) {
        int sum = *(a->data + i) + *(b->data + i);
        if ((*(a->data + i) > 0 && *(b->data + i) > 0 && sum < 0) ||
            (*(a->data + i) < 0 && *(b->data + i) < 0 && sum > 0)) {
            printf("Integer overflow detected in addition.\n");
            return;
        }
        *(result->data + i) = sum;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = *(a->data + i * a->cols + k) * *(b->data + k * b->cols + j);
                if (*(a->data + i * a->cols + k) != 0 && 
                    product / *(a->data + i * a->cols + k) != *(b->data + k * b->cols + j)) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                sum += product;
                if ((product > 0 && sum < 0) || (product < 0 && sum > 0)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
            }
            *(result->data + i * result->cols + j) = sum;
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
    init_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter number of columns (1-100): ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\nFirst Matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Addition:\n");
    add_matrices(&mat1, &mat2, &result);
    print_matrix(&result);
    
    printf("\nMatrix Multiplication:\n");
    multiply_matrices(&mat1, &mat2, &result);
    print_matrix(&result);
    
    return 0;
}