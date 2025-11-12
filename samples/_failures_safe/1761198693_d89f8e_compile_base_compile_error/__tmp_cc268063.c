//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 8

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[32];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            continue;
        }
        
        if (temp >= min && temp <= max && temp <= INT_MAX && temp >= INT_MIN) {
            value = (int)temp;
            break;
        }
    }
    return value;
}

void read_matrix(Matrix* mat) {
    mat->rows = read_int("Enter rows (1-8): ", 1, MAX_DIM);
    mat->cols = read_int("Enter cols (1-8): ", 1, MAX_DIM);
    
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int(prompt, -1000, 1000);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int recursive_trace(const Matrix* mat, int row, int col, int sum) {
    if (row >= mat->rows || col >= mat->cols) {
        return sum;
    }
    
    if (sum > INT_MAX - mat->data[row][col] || sum < INT_MIN + mat->data[row][col]) {
        return sum;
    }
    
    int new_sum = sum + mat->data[row][col];
    
    if (row == col) {
        return recursive_trace(mat, row + 1, col + 1, new_sum);
    }
    
    return new_sum;
}

int matrix_trace(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    return recursive_trace(mat, 0, 0, 0);
}

void recursive_transpose(const Matrix* src, Matrix* dst, int row, int col) {
    if (row >= src->rows) {
        return;
    }
    
    if (col >= src->cols) {
        recursive_transpose(src, dst, row + 1, 0);
        return;
    }
    
    dst->data[col][row] = src->data[row][col];
    recursive_transpose(src, dst, row, col + 1);
}

void matrix_transpose(const Matrix* src, Matrix* dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    recursive_transpose(src, dst, 0, 0);
}

int recursive_determinant(const Matrix* mat, int size, int* used_cols, int row) {
    if (row >= size) {
        return 1;
    }
    
    int det = 0;
    int sign = 1;
    
    for (int col = 0; col < size; col++) {
        if (used_cols[col]) {
            continue;
        }
        
        used_cols[col] = 1;
        
        int minor_det = recursive_determinant(mat, size, used_cols, row + 1);
        
        int product;
        if (mat->data[row][col] > 0 && minor_det > INT_MAX / mat->data[row][col]) {
            used_cols[col] = 0;
            continue;
        }
        if (mat->data[row][col] < 0 && minor_det < INT_MIN / mat->data[row][col]) {
            used_cols[col] = 0;
            continue;
        }
        product = mat->data[row][col] * minor_det;
        
        int signed_product;
        if (sign > 0) {
            signed_product = product;
        } else {
            if (product == INT_MIN) {
                used_cols[col] = 0;
                continue;
            }
            signed_product = -product;
        }
        
        if (det > 0 && signed_product > INT_MAX - det) {
            used_cols[col] = 0;
            continue;
        }
        if (det < 0 && signed_product < INT_MIN - det) {
            used_cols[col] = 0;
            continue;
        }
        
        det += signed_product;
        sign = -sign;
        used_cols[col] = 0;
    }
    
    return det;
}

int matrix_determinant(const Matrix* mat) {
    if (mat->rows != mat->cols) {
        return 0;
    }
    
    int size = mat->rows;
    if (size == 0) {
        return 1;
    }
    if (