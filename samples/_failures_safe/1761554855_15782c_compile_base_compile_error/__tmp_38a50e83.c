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
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            *(ptr + i * mat->cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(ptr + i * mat->cols + j));
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
    
    const int* ptr_a = a->data;
    const int* ptr_b = b->data;
    int* ptr_res = result->data;
    
    for (int i = 0; i < a->rows * a->cols; i++) {
        if ((*ptr_a > 0 && *ptr_b > INT_MAX - *ptr_a) ||
            (*ptr_a < 0 && *ptr_b < INT_MIN - *ptr_a)) {
            printf("Integer overflow detected in addition.\n");
            return;
        }
        *ptr_res = *ptr_a + *ptr_b;
        ptr_a++;
        ptr_b++;
        ptr_res++;
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    int* ptr_res = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int elem_a = *(a->data + i * a->cols + k);
                int elem_b = *(b->data + k * b->cols + j);
                
                long long product = (long long)elem_a * elem_b;
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                
                if ((elem_a > 0 && elem_b > 0 && sum > INT_MAX - (int)product) ||
                    (elem_a < 0 && elem_b < 0 && sum < INT_MIN - (int)product) ||
                    (sum > 0 && (int)product > INT_MAX - sum) ||
                    (sum < 0 && (int)product < INT_MIN - sum)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                
                sum += (int)product;
            }
            *(ptr_res + i * result->cols + j) = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    const int* ptr_src = src->data;
    int* ptr_dest = dest->data;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(ptr_dest + j * dest->cols + i) = *(ptr_src + i * src->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix 1:\n");
    mat1.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);
    mat1.cols =