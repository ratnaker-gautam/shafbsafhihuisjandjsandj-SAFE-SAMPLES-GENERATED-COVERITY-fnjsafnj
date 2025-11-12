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
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
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
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_read(Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "  [%d,%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int(prompt, -1000, 1000);
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (result->rows != a->rows || result->cols != b->cols) {
        return 0;
    }
    
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
                    (a_val > 0 && b_val < 0 && a_val > INT_MIN / b_val) ||
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

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    if (result->rows != a->rows || result->cols != a->cols) {
        return 0;
    }
    
    int* res_ptr = result->data;
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    
    for (int i = 0; i < a->rows * a->cols; i++) {
        int sum = *(a_ptr + i) + *(b_ptr + i);
        if ((*(a_ptr + i) > 0 && *(b_ptr + i) > 0 && sum < *(a_ptr + i)) ||
            (*(a_ptr + i) < 0 && *(b_ptr + i) < 0 && sum > *(a_ptr + i))) {
            return 0;
        }
        *(res_ptr + i) = sum;
    }
    
    return 1;
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    int rows_a = read_int("Enter rows for matrix A (1-16): ", 1, MAX_DIM);
    int cols_a = read_int("Enter columns for matrix A (1-16): ",