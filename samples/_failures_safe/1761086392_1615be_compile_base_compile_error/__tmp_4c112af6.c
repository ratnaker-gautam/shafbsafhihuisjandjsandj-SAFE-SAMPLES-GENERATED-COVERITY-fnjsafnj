//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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
            *ptr = read_int("", INT_MIN, INT_MAX);
            ptr++;
        }
    }
}

void print_matrix(const Matrix* mat) {
    const int* ptr = mat->data;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *ptr);
            ptr++;
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions do not match for addition.\n");
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
                const int* elem_a = a->data + i * a->cols + k;
                const int* elem_b = b->data + k * b->cols + j;
                
                if (*elem_a != 0 && *elem_b != 0) {
                    if ((*elem_a > 0 && *elem_b > 0 && *elem_a > INT_MAX / *elem_b) ||
                        (*elem_a < 0 && *elem_b < 0 && *elem_a < INT_MAX / *elem_b) ||
                        (*elem_a > 0 && *elem_b < 0 && *elem_b < INT_MIN / *elem_a) ||
                        (*elem_a < 0 && *elem_b > 0 && *elem_a < INT_MIN / *elem_b)) {
                        printf("Integer overflow detected in multiplication.\n");
                        return;
                    }
                }
                
                int product = *elem_a * *elem_b;
                if ((product > 0 && sum > INT_MAX - product) ||
                    (product < 0 && sum < INT_MIN - product)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                sum += product;
            }
            *ptr_res = sum;
            ptr_res++;
        }
    }
}

void transpose_matrix(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    int* ptr_res = result->data;
    for (int j = 0; j < mat->cols; j++) {
        for (int i = 0; i < mat->rows; i++) {
            const int* elem = mat->data + i * mat->cols + j;
            *ptr_res = *elem;
            ptr_res++;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter number of rows (1-100): ", 1, MAX_DIM);