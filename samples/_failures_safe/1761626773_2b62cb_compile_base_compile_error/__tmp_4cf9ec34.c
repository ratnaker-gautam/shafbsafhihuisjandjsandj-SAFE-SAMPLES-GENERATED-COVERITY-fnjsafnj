//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_MAX_SIZE 10
#define MATRIX_MIN_SIZE 1
#define MATRIX_ELEM_MAX 1000
#define MATRIX_ELEM_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUB 2
#define MATRIX_OP_MUL 3

typedef struct {
    int rows;
    int cols;
    int data[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
} Matrix;

static int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

static void matrix_init(Matrix* mat) {
    memset(mat, 0, sizeof(Matrix));
}

static int matrix_read(Matrix* mat, const char* name) {
    printf("Enter dimensions for matrix %s:\n", name);
    mat->rows = read_int("Rows: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    if (mat->rows == -1) return 0;
    
    mat->cols = read_int("Columns: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    if (mat->cols == -1) return 0;
    
    printf("Enter %d elements for matrix %s:\n", mat->rows * mat->cols, name);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int val = read_int("", MATRIX_ELEM_MIN, MATRIX_ELEM_MAX);
            if (val == -1) return 0;
            mat->data[i][j] = val;
        }
    }
    return 1;
}

static void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

static int matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions must match for addition.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > MATRIX_ELEM_MAX || sum < MATRIX_ELEM_MIN) {
                printf("Result element out of bounds.\n");
                return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

static int matrix_sub(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions must match for subtraction.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long diff = (long)a->data[i][j] - (long)b->data[i][j];
            if (diff > MATRIX_ELEM_MAX || diff < MATRIX_ELEM_MIN) {
                printf("Result element out of bounds.\n");
                return 0;
            }
            result->data[i][j] = (int)diff;
        }
    }
    return 1;
}

static int matrix_mul(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix A columns must match matrix B rows for multiplication.\n");
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] *