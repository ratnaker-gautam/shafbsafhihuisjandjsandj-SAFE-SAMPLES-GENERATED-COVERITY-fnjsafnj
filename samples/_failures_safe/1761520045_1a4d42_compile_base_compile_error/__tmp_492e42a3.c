//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUB 2
#define MATRIX_OP_MUL 3

#define SAFE_ADD(a, b) (((b) > 0) && ((a) > INT_MAX - (b))) ? INT_MAX : (((b) < 0) && ((a) < INT_MIN - (b))) ? INT_MIN : ((a) + (b))
#define SAFE_MUL(a, b) (((a) > 0) && ((b) > 0) && ((a) > INT_MAX / (b))) ? INT_MAX : (((a) > 0) && ((b) < 0) && ((b) < INT_MIN / (a))) ? INT_MIN : (((a) < 0) && ((b) > 0) && ((a) < INT_MIN / (b))) ? INT_MIN : (((a) < 0) && ((b) < 0) && ((a) < INT_MAX / (b))) ? INT_MAX : ((a) * (b))

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

void init_matrix(Matrix* mat) {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            mat->data[i][j] = 0;
        }
    }
    mat->rows = 0;
    mat->cols = 0;
}

int read_matrix(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    mat->cols = read_int("Columns: ", MIN_DIM, MAX_COLS);
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("", INT_MIN, INT_MAX);
        }
    }
    return 1;
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = SAFE_ADD(a->data[i][j], b->data[i][j]);
        }
    }
    return 1;
}

int matrix_sub(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = SAFE_ADD(a->data[i][j], -b->data[i][j]);
        }
    }
    return 1;
}

int matrix_mul(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (