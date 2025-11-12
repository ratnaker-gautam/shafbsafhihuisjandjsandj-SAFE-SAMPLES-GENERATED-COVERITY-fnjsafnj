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

void init_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        char prompt[32];
        snprintf(prompt, sizeof(prompt), "Element [%d,%d]: ", 
                i / mat->cols, i % mat->cols);
        *(mat->data + i) = read_int(prompt, INT_MIN, INT_MAX);
    }
}

void print_matrix(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(mat->data + i * mat->cols + j));
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
    int size = a->rows * a->cols;
    
    for (int i = 0; i < size; i++) {
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
    
    print_matrix(result);
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    int* res_ptr = result->data;
    int total_size = result->rows * result->cols;
    
    for (int i = 0; i < total_size; i++) {
        *res_ptr = 0;
        res_ptr++;
    }
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
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
                
                int product = a_val * b_val;
                
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                
                sum += product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
    
    print_matrix(result);
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-10): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter number of columns (1-10): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    mat2.rows = read_int("Enter number of rows (1-10): ", 1, MAX_DIM