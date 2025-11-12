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

#define SAFE_MULT(a, b, max_val) ((b) != 0 && ((a) > (max_val) / (b) || (a) < (-(max_val)) / (b))) ? 0 : ((a) * (b))

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
        return INT_MIN;
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return INT_MIN;
    }
    
    if (value < min_val || value > max_val) {
        return INT_MIN;
    }
    
    return (int)value;
}

int read_matrix_element(int row, int col) {
    char prompt[64];
    snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", row, col);
    return read_int(prompt, MATRIX_ELEMENT_MIN, MATRIX_ELEMENT_MAX);
}

int initialize_matrix(Matrix* mat) {
    mat->rows = read_int("Enter number of rows (1-10): ", 1, MAX_SIZE);
    if (mat->rows == INT_MIN) return 0;
    
    mat->cols = read_int("Enter number of columns (1-10): ", 1, MAX_SIZE);
    if (mat->cols == INT_MIN) return 0;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            int element = read_matrix_element(i, j);
            if (element == INT_MIN) return 0;
            mat->data[i][j] = element;
        }
    }
    return 1;
}

void print_matrix(const Matrix* mat) {
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
            if (sum > MATRIX_ELEMENT_MAX || sum < MATRIX_ELEMENT_MIN) return 0;
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
            if (diff > MATRIX_ELEMENT_MAX || diff < MATRIX_ELEMENT_MIN) return 0;
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
                long product = SAFE_MULT(a->data[i][k], b->data[k][j], MATRIX_ELEMENT_MAX);
                if (product == 0 && a->data[i][k] != 0 && b->data[k][j] != 0) return 0;
                sum += product;
                if (sum > MATRIX_ELEMENT_MAX || sum < MATRIX_ELEMENT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int main(void) {
    Matrix mat1, mat2, result;
    int operation;
    
    printf("Matrix Operations Program\n");