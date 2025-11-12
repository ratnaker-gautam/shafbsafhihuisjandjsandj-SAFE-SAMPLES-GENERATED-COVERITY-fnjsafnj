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

void read_matrix(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows: ", 1, MAX_DIM);
    mat->cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int* element = mat->data + i * mat->cols + j;
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            *element = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* element = mat->data + i * mat->cols + j;
            printf("%6d ", *element);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int* res_element = result->data + i * result->cols + j;
            *res_element = 0;
            
            for (int k = 0; k < a->cols; k++) {
                const int* a_element = a->data + i * a->cols + k;
                const int* b_element = b->data + k * b->cols + j;
                
                if (*a_element > 0 && *b_element > 0) {
                    if (*a_element > INT_MAX / *b_element) {
                        return -1;
                    }
                } else if (*a_element < 0 && *b_element < 0) {
                    if (*a_element < INT_MAX / *b_element) {
                        return -1;
                    }
                } else if (*a_element != 0 && *b_element != 0) {
                    if (*a_element < INT_MIN / *b_element) {
                        return -1;
                    }
                }
                
                int product = *a_element * *b_element;
                if ((*res_element > 0 && product > INT_MAX - *res_element) ||
                    (*res_element < 0 && product < INT_MIN - *res_element)) {
                    return -1;
                }
                
                *res_element += product;
            }
        }
    }
    
    return 1;
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
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
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    read_matrix(&mat1, "A");
    read_matrix(&mat2, "B");
    
    printf("\n");
    print_matrix(&mat1, "A");
    printf("\n");
    print_matrix(&mat2, "B");
    printf("\n");
    
    printf("Matrix Multiplication (A * B):\n");
    int mult_result = multiply_matrices(&mat1, &mat2, &result);
    if (mult_result == 1) {
        print_matrix(&result, "A * B");
    } else if