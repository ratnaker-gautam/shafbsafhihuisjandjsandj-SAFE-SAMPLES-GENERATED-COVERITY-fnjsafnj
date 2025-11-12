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
    if (rows <= 0 || rows > MAX_DIM || cols <= 0 || cols > MAX_DIM) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat) {
    if (mat == NULL || mat->rows <= 0 || mat->cols <= 0) {
        return;
    }
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int* elem = mat->data + i * mat->cols + j;
            *elem = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat) {
    if (mat == NULL || mat->rows <= 0 || mat->cols <= 0) {
        return;
    }
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* elem = mat->data + i * mat->cols + j;
            printf("%6d ", *elem);
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
    
    if (a->rows > MAX_DIM || b->cols > MAX_DIM || a->cols > MAX_DIM || b->rows > MAX_DIM) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                const int* a_elem = a->data + i * a->cols + k;
                const int* b_elem = b->data + k * b->cols + j;
                sum += (*a_elem) * (*b_elem);
            }
            int* res_elem = result->data + i * result->cols + j;
            *res_elem = sum;
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* src, Matrix* dest) {
    if (src == NULL || dest == NULL) {
        return 0;
    }
    if (src->rows > MAX_DIM || src->cols > MAX_DIM) {
        return 0;
    }
    
    matrix_init(dest, src->cols, src->rows);
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            const int* src_elem = src->data + i * src->cols + j;
            int* dest_elem = dest->data + j * dest->cols + i;
            *dest_elem = *src_elem;
        }
    }
    
    return 1;
}

int main() {
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = read_int("Enter rows for first matrix (1-100): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for first matrix (1-100): ", 1, MAX_DIM);
    
    Matrix mat1;
    matrix_init(&mat1, rows1, cols1);
    matrix_input(&mat1);
    
    printf("\n");
    int rows2 = read_int("Enter rows for second matrix (1-100): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for second matrix (1-100): ", 1, MAX_DIM);
    
    Matrix mat2;
    matrix_init