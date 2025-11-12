//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 16

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
        char prompt[32];
        snprintf(prompt, sizeof(prompt), "Element [%d,%d]: ", 
                 i / mat->cols, i % mat->cols);
        *(mat->data + i) = read_int(prompt, INT_MIN, INT_MAX);
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
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
                sum += *(a->data + i * a->cols + k) * *(b->data + k * b->cols + j);
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest->data + j * dest->cols + i) = *(src->data + i * src->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First matrix:\n");
    mat1.rows = read_int("Enter rows (1-16): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns (1-16): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    mat2.rows = read_int("Enter rows (1-16): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns (1-16): ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\n");
    print_matrix(&mat1);
    printf("\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Multiplication:\n");
    multiply_matrices(&mat1, &mat2, &result);
    print_matrix(&result);
    
    printf("\nTranspose of first matrix:\n");
    transpose_matrix(&mat1, &result);
    print_matrix(&result);
    
    printf("\nTranspose of second matrix:\n");
    transpose_matrix(&mat2, &result);
    print_matrix(&result);
    
    return 0;
}