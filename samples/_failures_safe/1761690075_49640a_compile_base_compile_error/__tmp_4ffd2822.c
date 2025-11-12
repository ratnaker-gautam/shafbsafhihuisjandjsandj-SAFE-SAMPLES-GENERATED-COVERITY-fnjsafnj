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

void init_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) {
                exit(1);
            }
            *(ptr++) = val;
        }
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr++));
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
    
    int* res_ptr = result->data;
    const int* a_ptr = a->data;
    
    for (int i = 0; i < a->rows; i++) {
        const int* b_ptr = b->data;
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row_ptr = a_ptr;
            const int* b_col_ptr = b_ptr;
            
            for (int k = 0; k < a->cols; k++) {
                if ((*a_row_ptr > 0 && *b_col_ptr > 0 && *a_row_ptr > INT_MAX / *b_col_ptr) ||
                    (*a_row_ptr < 0 && *b_col_ptr < 0 && *a_row_ptr < INT_MAX / *b_col_ptr) ||
                    (*a_row_ptr > 0 && *b_col_ptr < 0 && *a_row_ptr > INT_MIN / *b_col_ptr) ||
                    (*a_row_ptr < 0 && *b_col_ptr > 0 && *a_row_ptr < INT_MIN / *b_col_ptr)) {
                    return -1;
                }
                int product = *a_row_ptr * *b_col_ptr;
                if ((*a_row_ptr > 0 && *b_col_ptr > 0 && product < 0) ||
                    (*a_row_ptr < 0 && *b_col_ptr < 0 && product < 0) ||
                    (*a_row_ptr > 0 && *b_col_ptr < 0 && product > 0) ||
                    (*a_row_ptr < 0 && *b_col_ptr > 0 && product > 0)) {
                    return -1;
                }
                if (sum > 0 && product > INT_MAX - sum) return -1;
                if (sum < 0 && product < INT_MIN - sum) return -1;
                sum += product;
                a_row_ptr++;
                b_col_ptr += b->cols;
            }
            *(res_ptr++) = sum;
            b_ptr++;
        }
        a_ptr += a->cols;
    }
    
    return 1;
}

int transpose_matrix(const Matrix* src, Matrix* dst) {
    dst->rows = src->cols;
    dst->cols = src->rows;
    
    int* dst_ptr = dst->data;
    const int* src_ptr = src->data;
    
    for (int i = 0; i < src->cols; i++) {
        const int* col_ptr = src_ptr + i;
        for (int j = 0; j < src->rows; j++) {
            *(dst_ptr++) = *col_ptr;
            col_ptr += src->cols;
        }
    }
    
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix 1 dimensions:\n");
    mat1.rows = read_int("Rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Columns (1-100): ", 1, MAX_DIM);
    if (mat1.rows == -1 || mat1.cols == -1) return 1;
    
    init_matrix(&mat1);
    
    printf("Matrix 2 dimensions:\n");
    mat2.rows = read_int("Rows