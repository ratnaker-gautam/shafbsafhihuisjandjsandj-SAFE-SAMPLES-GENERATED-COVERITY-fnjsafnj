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

void matrix_init(Matrix* mat) {
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat) {
    mat->rows = read_int("Enter number of rows (1-16): ", 1, MAX_DIM);
    mat->cols = read_int("Enter number of columns (1-16): ", 1, MAX_DIM);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int* element = mat->data + i * mat->cols + j;
            *element = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* element = mat->data + i * mat->cols + j;
            printf("%6d", *element);
        }
        printf("\n");
    }
}

void matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrices must have same dimensions for addition.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            const int* elem_a = a->data + i * a->cols + j;
            const int* elem_b = b->data + i * b->cols + j;
            int* elem_result = result->data + i * result->cols + j;
            
            if ((*elem_b > 0 && *elem_a > INT_MAX - *elem_b) ||
                (*elem_b < 0 && *elem_a < INT_MIN - *elem_b)) {
                printf("Integer overflow detected in addition.\n");
                return;
            }
            *elem_result = *elem_a + *elem_b;
        }
    }
}

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix A columns must equal Matrix B rows for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    matrix_init(result);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int* elem_result = result->data + i * result->cols + j;
            
            for (int k = 0; k < a->cols; k++) {
                const int* elem_a = a->data + i * a->cols + k;
                const int* elem_b = b->data + k * b->cols + j;
                
                long long product = (long long)(*elem_a) * (long long)(*elem_b);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected in multiplication.\n");
                    return;
                }
                
                if ((*elem_result > 0 && product > INT_MAX - *elem_result) ||
                    (*elem_result < 0 && product < INT_MIN - *elem_result)) {
                    printf("Integer overflow detected in accumulation.\n");
                    return;
                }
                
                *elem_result += (int)product;
            }
        }
    }
}

void matrix_transpose(const Matrix* mat, Matrix* result) {
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            const int* src = mat->data + i * mat->cols + j;
            int* dest = result->data + j * result->cols + i;
            *dest = *src;
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    printf("Matrix A:\n");
    matrix_input(&a);
    
    printf("Matrix B