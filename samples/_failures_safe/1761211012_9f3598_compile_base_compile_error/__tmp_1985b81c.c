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
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

void matrix_init(Matrix* mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_print(const Matrix* mat) {
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%4d", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
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

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    
    int* res_ptr = result->data;
    int* a_ptr = a->data;
    int* b_ptr = b->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a_ptr + i * a->cols + k);
                int b_val = *(b_ptr + k * b->cols + j);
                long long product = (long long)a_val * b_val;
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                if ((a_val > 0 && b_val > 0 && sum > INT_MAX - a_val * b_val) ||
                    (a_val < 0 && b_val < 0 && sum < INT_MIN - a_val * b_val) ||
                    (a_val * b_val > 0 && sum > INT_MAX - a_val * b_val) ||
                    (a_val * b_val < 0 && sum < INT_MIN - a_val * b_val)) {
                    return 0;
                }
                sum += a_val * b_val;
            }
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dest) {
    if (src->rows != dest->cols || src->cols != dest->rows) {
        return 0;
    }
    
    int* src_ptr = src->data;
    int* dest_ptr = dest->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
        }
    }
    
    return 1;
}

int main() {
    Matrix a, b, result, transposed;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for matrix A (1-16): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix A (1-16): ", 1, MAX_DIM);
    matrix_init(&a, rows1, cols1);
    matrix_input(&a);
    
    int rows2 = read_int("Enter rows for matrix B (1-16): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix B (1-16): ", 1, MAX_DIM);
    matrix_init(&b, rows2, cols2);
    matrix_input(&b);
    
    printf("\nMatrix A:\n");
    matrix_print(&a);
    
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    printf("\nMatrix Multiplication:\n");
    if (matrix_multiply(&a, &b