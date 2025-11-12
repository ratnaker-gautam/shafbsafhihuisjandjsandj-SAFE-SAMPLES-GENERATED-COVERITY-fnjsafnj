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

void matrix_init(Matrix* mat, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        mat->rows = 0;
        mat->cols = 0;
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat, const char* name) {
    if (mat->rows <= 0 || mat->cols <= 0) {
        return;
    }
    printf("Enter values for matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  [%d][%d]: ", i, j);
            int* elem = mat->data + i * mat->cols + j;
            *elem = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    if (mat->rows <= 0 || mat->cols <= 0) {
        printf("Matrix %s is invalid\n", name);
        return;
    }
    printf("Matrix %s (%d x %d):\n", name, mat->rows, mat->cols);
    
    for (int i = 0; i < mat->rows; i++) {
        printf("  ");
        for (int j = 0; j < mat->cols; j++) {
            const int* elem = mat->data + i * mat->cols + j;
            printf("%6d ", *elem);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) {
        return 0;
    }
    if (a->cols != b->rows) {
        return 0;
    }
    
    if (a->rows <= 0 || a->cols <= 0 || b->rows <= 0 || b->cols <= 0) {
        return 0;
    }
    
    if (a->rows > MAX_DIM || b->cols > MAX_DIM) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
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
                
                if ((*a_elem > 0 && *b_elem > 0 && product < 0) ||
                    (*a_elem < 0 && *b_elem < 0 && product < 0) ||
                    (*a_elem > 0 && *b_elem < 0 && product > 0) ||
                    (*a_elem < 0 && *b_elem > 0 && product > 0)) {
                    return 0;
                }
                
                if (sum > 0 && product > INT_MAX - sum) {
                    return 0;
                }
                if (sum < 0 && product < INT_MIN - sum) {
                    return 0;
                }
                
                sum