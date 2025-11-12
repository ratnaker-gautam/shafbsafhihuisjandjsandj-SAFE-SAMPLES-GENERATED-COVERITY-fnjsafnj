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
    const int* a_row_ptr = a->data;
    
    for (int i = 0; i < a->rows; i++) {
        const int* b_col_ptr = b->data;
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            const int* a_elem_ptr = a_row_ptr;
            const int* b_elem_ptr = b_col_ptr;
            
            for (int k = 0; k < a->cols; k++) {
                if (*a_elem_ptr > 0 && *b_elem_ptr > 0) {
                    if (*a_elem_ptr > INT_MAX / *b_elem_ptr) {
                        return 0;
                    }
                } else if (*a_elem_ptr < 0 && *b_elem_ptr < 0) {
                    if (*a_elem_ptr < INT_MAX / *b_elem_ptr) {
                        return 0;
                    }
                } else if (*a_elem_ptr != 0 && *b_elem_ptr != 0) {
                    if ((*a_elem_ptr == -1 && *b_elem_ptr == INT_MIN) ||
                        (*b_elem_ptr == -1 && *a_elem_ptr == INT_MIN)) {
                        return 0;
                    }
                }
                
                sum += *a_elem_ptr * *b_elem_ptr;
                
                if ((*a_elem_ptr > 0 && *b_elem_ptr > 0 && sum < 0) ||
                    (*a_elem_ptr < 0 && *b_elem_ptr < 0 && sum < 0) ||
                    (*a_elem_ptr > 0 && *b_elem_ptr < 0 && sum > 0) ||
                    (*a_elem_ptr < 0 && *b_elem_ptr > 0 && sum > 0)) {
                    return 0;
                }
                
                a_elem_ptr++;
                b_elem_ptr += b->cols;
            }
            
            *res_ptr = sum;
            res_ptr++;
            b_col_ptr++;
        }
        a_row_ptr += a->cols;
    }
    
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    int* dest_ptr = dest->data;
    const int* src_col_ptr = src->data;
    
    for (int j = 0; j < src->cols; j++) {
        const int* src_row_ptr = src_col_ptr;
        for (int i = 0; i < src->rows; i++) {
            *dest_ptr = *src_row_ptr;
            dest_ptr++;
            src_row_ptr += src->cols;
        }
        src