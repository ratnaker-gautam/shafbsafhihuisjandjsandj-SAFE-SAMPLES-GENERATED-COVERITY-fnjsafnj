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
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        mat->rows = 1;
        mat->cols = 1;
    } else {
        mat->rows = rows;
        mat->cols = cols;
    }
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat) {
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int("", INT_MIN, INT_MAX);
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
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    if (a->cols != b->rows) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* r_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a_ptr + i * a->cols + k);
                int b_val = *(b_ptr + k * b->cols + j);
                if ((a_val > 0 && b_val > 0 && a_val > INT_MAX / b_val) ||
                    (a_val > 0 && b_val < 0 && b_val < INT_MIN / a_val) ||
                    (a_val < 0 && b_val > 0 && a_val < INT_MIN / b_val) ||
                    (a_val < 0 && b_val < 0 && a_val < INT_MAX / b_val)) {
                    return 0;
                }
                int product = a_val * b_val;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    return 0;
                }
                sum += product;
            }
            *(r_ptr + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dest) {
    if (src == NULL || dest == NULL) {
        return 0;
    }
    if (src->rows != dest->cols || src->cols != dest->rows) {
        return 0;
    }
    
    const int* src_ptr = src->data;
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
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        printf("Matrix A:\n");
        int rows_a = read_int("Enter rows: ", 1, MAX_DIM);
        int cols_a = read_int("Enter columns: ", 1, MAX_DIM);
        matrix_init(&a, rows_a, cols_a);
        matrix_input(&a);
        
        printf("Matrix B:\n");
        int rows_b = read_int("Enter rows: ", 1, MAX_DIM);
        int