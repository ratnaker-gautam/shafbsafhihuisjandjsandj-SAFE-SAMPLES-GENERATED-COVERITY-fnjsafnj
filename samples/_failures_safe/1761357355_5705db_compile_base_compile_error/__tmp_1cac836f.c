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

void read_matrix(Matrix* mat) {
    printf("Enter matrix elements:\n");
    int* ptr = mat->data;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            int value = read_int("", INT_MIN, INT_MAX);
            if (value == -1) {
                printf("Input error.\n");
                exit(1);
            }
            *(ptr + i * mat->cols + j) = value;
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
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int idx = i * a->cols + j;
            if ((*(a_ptr + idx) > 0 && *(b_ptr + idx) > INT_MAX - *(a_ptr + idx)) ||
                (*(a_ptr + idx) < 0 && *(b_ptr + idx) < INT_MIN - *(a_ptr + idx))) {
                printf("Integer overflow detected.\n");
                exit(1);
            }
            *(res_ptr + idx) = *(a_ptr + idx) + *(b_ptr + idx);
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    const int* a_ptr = a->data;
    const int* b_ptr = b->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            
            for (int k = 0; k < a->cols; k++) {
                long product = (long)*(a_ptr + i * a->cols + k) * (long)*(b_ptr + k * b->cols + j);
                
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                
                if ((product > 0 && sum > LONG_MAX - product) ||
                    (product < 0 && sum < LONG_MIN - product)) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                
                sum += product;
            }
            
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Integer overflow detected.\n");
                exit(1);
            }
            
            *(res_ptr + i * result->cols + j) = (int)sum;
        }
    }
}

void transpose_matrix(const Matrix* input, Matrix* result) {
    const int* in_ptr = input->data;
    int* res_ptr = result->data;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            *(res_ptr + j * result->cols + i) = *(in_ptr + i * input->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    mat1.rows = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.rows == -1) return 1;
    
    mat1.cols = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    if (mat1.cols == -1) return 1;
    
    read_matrix(&mat1);
    
    printf("\