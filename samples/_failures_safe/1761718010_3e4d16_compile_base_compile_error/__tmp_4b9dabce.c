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

void matrix_init(Matrix* mat, int rows, int cols) {
    if (rows <= 0 || rows > MAX_DIM || cols <= 0 || cols > MAX_DIM) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat) {
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat) {
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
    
    matrix_init(result, a->rows, b->cols);
    int* res_ptr = result->data;
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    
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
                    return 0;
                }
                int product = a_val * b_val;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dest) {
    matrix_init(dest, src->cols, src->rows);
    int* dest_ptr = dest->data;
    const int* src_ptr = src->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
        }
    }
    
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix 1 dimensions:\n");
    int rows1 = read_int("Rows: ", 1, MAX_DIM);
    int cols1 = read_int("Columns: ", 1, MAX_DIM);
    matrix_init(&mat1, rows1, cols1);
    matrix_input(&mat1);
    
    printf("\nMatrix 2 dimensions:\n");
    int rows2 = read_int("Rows: ", 1, MAX_DIM);
    int cols2 = read_int("Columns: ", 1, MAX_DIM);
    matrix_init(&mat2, rows2, cols2);
    matrix_input(&mat2);
    
    printf("\nMatrix 1:\n");
    matrix_print(&mat1);
    
    printf("\nMatrix 2:\n");
    matrix_print(&mat2);
    
    printf("\nMatrix Multiplication:\