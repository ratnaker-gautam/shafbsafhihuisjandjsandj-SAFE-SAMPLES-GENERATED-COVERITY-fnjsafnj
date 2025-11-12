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
    return (dim > 0 && dim <= MAX_DIM);
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

void matrix_init(Matrix* mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat) {
    printf("Enter matrix elements:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            int* element = mat->data + i * mat->cols + j;
            *element = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat) {
    printf("Matrix (%dx%d):\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* element = mat->data + i * mat->cols + j;
            printf("%6d ", *element);
        }
        printf("\n");
    }
}

void matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions don't match for addition.\n");
        return;
    }
    
    matrix_init(result, a->rows, a->cols);
    
    int size = a->rows * a->cols;
    for (int i = 0; i < size; i++) {
        int sum = *(a->data + i) + *(b->data + i);
        if ((*(a->data + i) > 0 && *(b->data + i) > 0 && sum < 0) ||
            (*(a->data + i) < 0 && *(b->data + i) < 0 && sum > 0)) {
            printf("Integer overflow detected in addition.\n");
            return;
        }
        *(result->data + i) = sum;
    }
}

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    
    matrix_init(result, a->rows, b->cols);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                const int* a_element = a->data + i * a->cols + k;
                const int* b_element = b->data + k * b->cols + j;
                
                long product = (long)(*a_element) * (long)(*b_element);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                
                long new_sum = (long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    printf("Integer overflow detected in summation.\n");
                    return;
                }
                
                sum = (int)new_sum;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    matrix_init(dest, src->cols, src->rows);
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            const int* src_element = src->data + i * src->cols + j;
            int* dest_element = dest->data + j * dest->cols + i;
            *dest_element = *src_element;
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    int cols1 = read_int