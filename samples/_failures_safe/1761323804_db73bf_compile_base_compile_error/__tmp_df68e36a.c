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

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[32];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void init_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        int* element = mat->data + i;
        char prompt[32];
        snprintf(prompt, sizeof(prompt), "Element [%d,%d]: ", 
                 i / mat->cols, i % mat->cols);
        *element = read_int(prompt, INT_MIN, INT_MAX);
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions must match for addition.\n");
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
        printf("Invalid dimensions for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int* res_ptr = result->data + i * result->cols + j;
            *res_ptr = 0;
            
            for (int k = 0; k < a->cols; k++) {
                int a_val = *(a->data + i * a->cols + k);
                int b_val = *(b->data + k * b->cols + j);
                
                if (a_val != 0 && b_val != 0) {
                    if ((a_val > 0 && b_val > INT_MAX / a_val) ||
                        (a_val < 0 && b_val < INT_MAX / a_val) ||
                        (a_val > 0 && b_val < INT_MIN / a_val) ||
                        (a_val < 0 && b_val > INT_MIN / a_val)) {
                        printf("Integer overflow detected in multiplication.\n");
                        return;
                    }
                }
                
                long product = (long)a_val * (long)b_val;
                if ((product > 0 && *res_ptr > INT_MAX - product) ||
                    (product < 0 && *res_ptr < INT_MIN - product)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                
                *res_ptr += a_val * b_val;
            }
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("Maximum dimension: %d\n", MAX_DIM);
    
    mat1.rows = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    mat2.rows = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat