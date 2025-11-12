//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define SAFE_MULT(a, b) \
    (((b) != 0 && (a) > INT_MAX / (b)) || ((b) != 0 && (a) < INT_MIN / (b))) ? 0 : (a) * (b)

typedef struct {
    int data[MAX_SIZE][MAX_SIZE];
    int rows;
    int cols;
} Matrix;

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    
    if (value < min_val || value > max_val) {
        return -1;
    }
    
    return (int)value;
}

int read_matrix_element(int row, int col) {
    char prompt[64];
    snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", row, col);
    return read_int(prompt, MATRIX_ELEMENT_MIN, MATRIX_ELEMENT_MAX);
}

void initialize_matrix(Matrix* mat) {
    memset(mat->data, 0, sizeof(mat->data));
    mat->rows = 0;
    mat->cols = 0;
}

int input_matrix(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    
    mat->rows = read_int("Rows: ", 1, MAX_SIZE);
    if (mat->rows == -1) return 0;
    
    mat->cols = read_int("Columns: ", 1, MAX_SIZE);
    if (mat->cols == -1) return 0;
    
    printf("Enter elements for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int element = read_matrix_element(i, j);
            if (element == -1) return 0;
            mat->data[i][j] = element;
        }
    }
    return 1;
}

void print_matrix(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int matrix_subtract(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long diff = (long)a->data[i][j] - b->data[i][j];
            if (diff > INT_MAX || diff < INT_MIN) return 0;
            result->data[i][j] = (int)diff;
        }
    }
    return 1;
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = SAFE_MULT(a->data[i][k], b->data[k][j]);
                if (product == 0 && (a->data[i][k] != 0 && b->data[k][j] != 0)) return 0;
                sum += product;