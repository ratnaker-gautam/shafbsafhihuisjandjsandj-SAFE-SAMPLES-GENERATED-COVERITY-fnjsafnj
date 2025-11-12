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
    printf("Enter values for matrix (%d x %d):\n", mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(mat->data + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
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

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += (*(a->data + i * a->cols + k)) * (*(b->data + k * b->cols + j));
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
    
    return 1;
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
    printf("=========================\n\n");
    
    printf("Matrix 1 dimensions:\n");
    mat1.rows = read_int("Rows: ", 1, MAX_DIM);
    mat1.cols = read_int("Columns: ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\nMatrix 2 dimensions:\n");
    mat2.rows = read_int("Rows: ", 1, MAX_DIM);
    mat2.cols = read_int("Columns: ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&mat1, &mat2, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply: incompatible dimensions.\n");
    }
    
    printf("\nTranspose of Matrix 1:\n");
    transpose_matrix(&mat1, &result);
    print_matrix(&result);
    
    printf("\nTranspose of Matrix 2:\n");
    transpose_matrix(&mat2, &result);
    print_matrix(&result);
    
    return 0;
}