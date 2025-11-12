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

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char *endptr;
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

void matrix_init(Matrix *mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_print(const Matrix *mat) {
    int *ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%4d", *(ptr + i * mat->cols + j));
        }
        printf("\n");
    }
}

void matrix_input(Matrix *mat) {
    printf("Enter matrix elements row by row:\n");
    int *ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (a->rows > MAX_DIM || b->cols > MAX_DIM) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    int *res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            int *a_row = a->data + i * a->cols;
            int *b_col = b->data + j;
            
            for (int k = 0; k < a->cols; k++) {
                if (sum > 0 && *(a_row + k) > 0 && sum > INT_MAX - *(a_row + k) * *(b_col + k * b->cols)) {
                    return 0;
                }
                if (sum < 0 && *(a_row + k) < 0 && sum < INT_MIN - *(a_row + k) * *(b_col + k * b->cols)) {
                    return 0;
                }
                sum += *(a_row + k) * *(b_col + k * b->cols);
            }
            
            *(res_ptr + i * result->cols + j) = sum;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix *src, Matrix *dest) {
    if (src->rows > MAX_DIM || src->cols > MAX_DIM) {
        return 0;
    }
    
    matrix_init(dest, src->cols, src->rows);
    int *src_ptr = src->data;
    int *dest_ptr = dest->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest_ptr + j * dest->cols + i) = *(src_ptr + i * src->cols + j);
        }
    }
    
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    
    choice = read_int("Select operation (1-2): ", 1, 2);
    
    if (choice == 1) {
        printf("Matrix A:\n");
        int rows1 = read_int("Enter rows (1-16): ", 1, MAX_DIM);
        int cols1 = read_int("Enter columns (1-16): ", 1, MAX_DIM);
        matrix_init(&mat1, rows1, cols1);
        matrix_input(&mat1);
        
        printf("Matrix B:\n");
        int rows2 = read_int("Enter rows (1-16): ", 1, MAX_DIM);
        int cols2 = read_int("Enter columns (1-16): ", 1, MAX_DIM);
        matrix_init(&mat2, rows2, cols2);
        matrix_input(&mat2);
        
        printf("\nMatrix