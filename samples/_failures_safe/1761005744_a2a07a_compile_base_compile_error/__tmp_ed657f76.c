//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
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

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long long_val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (long_val < min || long_val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)long_val;
        break;
    }
    
    return value;
}

void matrix_init(Matrix* mat) {
    memset(mat->data, 0, sizeof(mat->data));
    mat->rows = 0;
    mat->cols = 0;
}

void matrix_input(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows: ", 1, MAX_DIM);
    mat->cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int* element = mat->data + i * mat->cols + j;
            *element = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* element = mat->data + i * mat->cols + j;
            printf("%6d ", *element);
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
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int* res_element = result->data + i * result->cols + j;
            *res_element = 0;
            
            for (int k = 0; k < a->cols; k++) {
                const int* a_element = a->data + i * a->cols + k;
                const int* b_element = b->data + k * b->cols + j;
                
                if (*a_element > 0 && *b_element > 0) {
                    if (*a_element > INT_MAX / *b_element) {
                        return 0;
                    }
                } else if (*a_element < 0 && *b_element < 0) {
                    if (*a_element < INT_MAX / *b_element) {
                        return 0;
                    }
                } else if (*a_element != 0 && *b_element != 0) {
                    if ((*a_element == -1 && *b_element == INT_MIN) ||
                        (*b_element == -1 && *a_element == INT_MIN)) {
                        return 0;
                    }
                }
                
                int product = *a_element * *b_element;
                if ((product > 0 && *res_element > INT_MAX - product) ||
                    (product < 0 && *res_element < INT_MIN - product)) {
                    return 0;
                }
                
                *res_element += product;
            }
        }
    }
    
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
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
    Matrix a, b, result;
    
    matrix_init(&a);
    matrix_init(&b);
    matrix_init(&result);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    matrix_input(&a, "A");
    matrix_input(&b, "B");
    
    printf("\n");
    matrix_print(&a, "A");
    printf("\n");
    matrix_print(&b, "B");
    printf("\n");
    
    printf("Matrix Multiplication (A * B):\n");
    if (matrix_multiply(&a, &b, &result)) {
        matrix_print(&result,