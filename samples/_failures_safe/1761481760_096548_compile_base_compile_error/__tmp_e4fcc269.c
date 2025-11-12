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

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

void matrix_init(Matrix* mat) {
    mat->rows = 0;
    mat->cols = 0;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_read(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows: ", 1, MAX_DIM);
    mat->cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int* elem = mat->data + i * mat->cols + j;
            *elem = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* elem = mat->data + i * mat->cols + j;
            printf("%6d ", *elem);
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
            int* res_elem = result->data + i * result->cols + j;
            *res_elem = 0;
            
            for (int k = 0; k < a->cols; k++) {
                const int* a_elem = a->data + i * a->cols + k;
                const int* b_elem = b->data + k * b->cols + j;
                
                if (*a_elem > 0 && *b_elem > 0) {
                    if (*a_elem > INT_MAX / *b_elem) {
                        return 0;
                    }
                } else if (*a_elem < 0 && *b_elem < 0) {
                    if (*a_elem < INT_MAX / *b_elem) {
                        return 0;
                    }
                } else if (*a_elem != 0 && *b_elem != 0) {
                    if ((*a_elem == -1 && *b_elem == INT_MIN) ||
                        (*b_elem == -1 && *a_elem == INT_MIN)) {
                        return 0;
                    }
                }
                
                int product = *a_elem * *b_elem;
                if ((*res_elem > 0 && product > INT_MAX - *res_elem) ||
                    (*res_elem < 0 && product < INT_MIN - *res_elem)) {
                    return 0;
                }
                *res_elem += product;
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
            const int* src_elem = src->data + i * src->cols + j;
            int* dest_elem = dest->data + j * dest->cols + i;
            *dest_elem = *src_elem;
        }
    }
}

int main() {
    Matrix a, b, result, transposed;
    
    matrix_init(&a);
    matrix_init(&b);
    matrix_init(&result);
    matrix_init(&transposed);
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    matrix_read(&a, "A");
    matrix_read(&b, "B");
    
    printf("\n");
    matrix_print(&a, "A");
    printf("\n");
    matrix_print(&b, "B");
    printf("\n");
    
    printf("Matrix Multiplication (A * B):\n");
    if (matrix_multiply(&a, &b, &result)) {
        matrix_print(&result, "A*B");
    } else {
        printf("Multiplication failed