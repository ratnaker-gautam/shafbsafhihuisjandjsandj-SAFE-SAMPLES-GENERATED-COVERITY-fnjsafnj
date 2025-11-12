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

#define MATRIX_SAFE_ADD(a, b) (((b) > 0) ? (((a) > (INT_MAX - (b))) ? INT_MAX : ((a) + (b))) : (((a) < (INT_MIN - (b))) ? INT_MIN : ((a) + (b))))
#define MATRIX_SAFE_SUB(a, b) (((b) > 0) ? (((a) < (INT_MIN + (b))) ? INT_MIN : ((a) - (b))) : (((a) > (INT_MAX + (b))) ? INT_MAX : ((a) - (b))))
#define MATRIX_SAFE_MUL(a, b) (((a) == 0 || (b) == 0) ? 0 : (((a) > 0) ? (((b) > 0) ? (((a) > (INT_MAX / (b))) ? INT_MAX : ((a) * (b))) : (((b) < (INT_MIN / (a))) ? INT_MIN : ((a) * (b)))) : (((b) > 0) ? (((a) < (INT_MIN / (b))) ? INT_MIN : ((a) * (b))) : (((a) < (INT_MAX / (b))) ? INT_MAX : ((a) * (b))))))

typedef struct {
    int rows;
    int cols;
    int data[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    if (rows < MATRIX_MIN_SIZE || rows > MATRIX_MAX_SIZE) return 0;
    if (cols < MATRIX_MIN_SIZE || cols > MATRIX_MAX_SIZE) return 0;
    return 1;
}

int validate_matrix_element(int elem) {
    return (elem >= MATRIX_ELEM_MIN && elem <= MATRIX_ELEM_MAX);
}

int read_matrix(Matrix *m) {
    char input[256];
    int rows, cols;
    
    printf("Enter number of rows: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &rows) != 1) return 0;
    
    printf("Enter number of columns: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    if (sscanf(input, "%d", &cols) != 1) return 0;
    
    if (!validate_matrix_size(rows, cols)) return 0;
    
    m->rows = rows;
    m->cols = cols;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            if (fgets(input, sizeof(input), stdin) == NULL) return 0;
            int elem;
            if (sscanf(input, "%d", &elem) != 1) return 0;
            if (!validate_matrix_element(elem)) return 0;
            m->data[i][j] = elem;
        }
    }
    return 1;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_add(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = MATRIX_SAFE_ADD(a->data[i][j], b->data[i][j]);
        }
    }
    return 1;
}

int matrix_sub(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = MATRIX_SAFE_SUB(a->data[i][j], b->data[i][j]);
        }
    }
    return 1;
}

int matrix_mul(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (