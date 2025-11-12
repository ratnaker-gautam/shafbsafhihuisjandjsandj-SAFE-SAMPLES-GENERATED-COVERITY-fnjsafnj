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

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
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
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Element [%d,%d]: ", 
                 i / mat->cols, i % mat->cols);
        *(mat->data + i) = read_int(prompt, INT_MIN, INT_MAX);
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
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)*(a->data + i * a->cols + k) * 
                              (long)*(b->data + k * b->cols + j);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                if ((sum > 0 && product > INT_MAX - sum) ||
                    (sum < 0 && product < INT_MIN - sum)) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                sum += (int)product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest->data + j * dest->cols + i) = *(src->data + i * src->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix 1:\n");
    mat1.rows = read_int("Enter rows (1-100): ", 1, MAX_DIM);
    mat1.cols = read_int("Enter columns (1-100): ", 1, MAX_DIM);
    init_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    mat2.rows = read_int("Enter rows (1-100): ", 1, MAX_DIM);
    mat2.cols = read_int("Enter columns (1-100): ", 1, MAX_DIM);
    init_matrix(&mat2);
    
    printf("\nMatrix