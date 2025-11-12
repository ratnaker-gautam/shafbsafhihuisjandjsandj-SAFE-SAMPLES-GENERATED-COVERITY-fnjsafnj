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
    printf("Initializing matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            int* elem = mat->data + i * mat->cols + j;
            *elem = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* elem = mat->data + i * mat->cols + j;
            printf("%6d ", *elem);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions don't match for addition.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    int* a_ptr = a->data;
    int* b_ptr = b->data;
    int* res_ptr = result->data;
    int total = a->rows * a->cols;
    
    for (int i = 0; i < total; i++) {
        if ((*a_ptr > 0 && *b_ptr > INT_MAX - *a_ptr) ||
            (*a_ptr < 0 && *b_ptr < INT_MIN - *a_ptr)) {
            printf("Integer overflow detected in addition.\n");
            return;
        }
        *res_ptr = *a_ptr + *b_ptr;
        a_ptr++;
        b_ptr++;
        res_ptr++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int* res_elem = result->data + i * result->cols + j;
            *res_elem = 0;
            
            for (int k = 0; k < a->cols; k++) {
                const int* a_elem = a->data + i * a->cols + k;
                const int* b_elem = b->data + k * b->cols + j;
                
                if (*a_elem != 0) {
                    if (*b_elem > 0) {
                        if (*a_elem > INT_MAX / *b_elem || *a_elem < INT_MIN / *b_elem) {
                            printf("Integer overflow detected in multiplication.\n");
                            return;
                        }
                    } else if (*b_elem < 0) {
                        if (*a_elem < INT_MAX / *b_elem || *a_elem > INT_MIN / *b_elem) {
                            printf("Integer overflow detected in multiplication.\n");
                            return;
                        }
                    }
                }
                
                long long product = (long long)(*a_elem) * (long long)(*b_elem);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                
                if (*res_elem > 0 && product > INT_MAX - *res_elem) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                if (*res_elem < 0 && product < INT_MIN - *res_elem) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                
                *res_elem += (int)product;
            }
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First matrix dimensions:\n");
    mat1.rows = read_int("Rows: ", 1, MAX_DIM);
    mat1.cols = read_int("Columns: