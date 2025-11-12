//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

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

void matrix_init(Matrix* mat) {
    mat->rows = 0;
    mat->cols = 0;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows: ", 1, MAX_DIM);
    mat->cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            *ptr = read_int("", INT_MIN, INT_MAX);
            ptr++;
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    const int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *ptr);
            ptr++;
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
    
    int* res_ptr = result->data;
    const int* a_ptr = a->data;
    
    for (int i = 0; i < a->rows; i++) {
        const int* b_ptr = b->data;
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_row_ptr = a_ptr;
            const int* b_col_ptr = b_ptr;
            
            for (int k = 0; k < a->cols; k++) {
                sum += *a_row_ptr * *b_col_ptr;
                a_row_ptr++;
                b_col_ptr += b->cols;
            }
            
            *res_ptr = sum;
            res_ptr++;
            b_ptr++;
        }
        a_ptr += a->cols;
    }
    
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    int* dest_ptr = dest->data;
    const int* src_ptr = src->data;
    
    for (int i = 0; i < src->cols; i++) {
        const int* src_col_ptr = src_ptr + i;
        for (int j = 0; j < src->rows; j++) {
            *dest_ptr = *src_col_ptr;
            dest_ptr++;
            src_col_ptr += src->cols;
        }
    }
}

int main() {
    Matrix mat_a, mat_b, result;
    matrix_init(&mat_a);
    matrix_init(&mat_b);
    matrix_init(&result);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    matrix_input(&mat_a, "A");
    matrix_input(&mat_b, "B");
    
    printf("\n");
    matrix_print(&mat_a, "A");
    printf("\n");
    matrix_print(&mat_b, "B");
    
    printf("\nMatrix Multiplication:\n");
    if (matrix_multiply(&mat_a, &mat_b, &result)) {
        matrix_print(&result, "A x B");
    } else {
        printf("Cannot multiply: incompatible dimensions.\n");
    }
    
    printf("\nMatrix Transposition:\n");
    matrix_transpose(&mat_a, &result);
    matrix_print(&result, "A^T");
    
    matrix_transpose(&mat_b, &result);
    matrix_print(&result, "B^T");
    
    return 0;
}