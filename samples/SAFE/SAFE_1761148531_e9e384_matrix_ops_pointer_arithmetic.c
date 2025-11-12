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
    return (dim > 0 && dim <= MAX_DIM);
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

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += *(a_ptr + i * a->cols + k) * *(b_ptr + k * b->cols + j);
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    const int* src_ptr = src->data;
    int* dest_ptr = dest->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
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
    
    printf("\nMatrix 2 dimensions:\n");
    mat2.rows = read_int("Rows: ", 1, MAX_DIM);
    mat2.cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("\n");
    read_matrix(&mat1);
    printf("\n");
    read_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    if (mat1.cols == mat2.rows) {
        result.rows = mat1.rows;
        result.cols = mat2.cols;
        multiply_matrices(&mat1, &mat2, &result);
        printf("\nMatrix Multiplication Result:\n");
        print_matrix(&result);
    } else {
        printf("\nCannot multiply matrices: incompatible dimensions\n");
    }
    
    if (mat1.rows <= MAX_DIM && mat1.cols <= MAX_DIM) {
        Matrix transposed;
        transposed.rows = mat1.cols;
        transposed.cols = mat1.rows;
        transpose_matrix(&mat1, &transposed);
        printf("\nTranspose of Matrix 1:\n");
        print_matrix(&transposed);
    }
    
    if (mat2.rows <= MAX_DIM && mat2.cols <= MAX_DIM) {
        Matrix transposed;
        transposed.rows = mat2.cols;
        transposed.cols = mat2.rows;
        transpose_matrix(&mat2, &transposed);
        printf("\nTranspose of Matrix 2:\n");
        print_matrix(&transposed);
    }
    
    return 0;
}