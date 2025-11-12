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
        long long_val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (long_val < min || long_val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)long_val;
        break;
    }
    
    return value;
}

void matrix_init(Matrix* mat) {
    mat->rows = 0;
    mat->cols = 0;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_read(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows: ", 1, MAX_DIM);
    mat->cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        printf("Element [%d][%d]: ", i / mat->cols, i % mat->cols);
        *(ptr + i) = read_int("", INT_MIN, INT_MAX);
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    const int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long)*(a_ptr + i * a->cols + k) * 
                                   (long)*(b_ptr + k * b->cols + j);
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                sum += (int)product;
                if ((sum > 0 && product < 0 && sum < INT_MIN + product) ||
                    (sum < 0 && product > 0 && sum > INT_MAX - product)) {
                    return 0;
                }
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
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
    
    matrix_init(&mat1);
    matrix_init(&mat2);
    matrix_init(&result);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    matrix_read(&mat1, "A");
    matrix_read(&mat2, "B");
    
    printf("\n");
    matrix_print(&mat1, "A");
    printf("\n");
    matrix_print(&mat2, "B");
    
    printf("\nMatrix Multiplication (A * B):\n");
    if (matrix_multiply(&mat1, &mat2, &result)) {
        matrix_print(&result, "A * B");
    } else {
        printf("Cannot multiply: dimension mismatch or integer overflow.\n");
    }
    
    printf("\nMatrix Transpose (A^T):\n");
    matrix_transpose(&mat1, &result);
    matrix_print(&result, "A^T");
    
    printf("\nMatrix Transpose (B^T):\n");
    matrix_transpose(&mat2, &