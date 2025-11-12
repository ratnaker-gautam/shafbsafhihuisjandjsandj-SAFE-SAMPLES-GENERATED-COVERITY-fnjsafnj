//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MATRIX_SIZE (MAX_ROWS * MAX_COLS)

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

#define MATRIX_INIT { .data = {{0}}, .rows = 0, .cols = 0 }

#define VALID_DIMENSIONS(r, c) ((r) > 0 && (r) <= MAX_ROWS && (c) > 0 && (c) <= MAX_COLS)
#define VALID_INDEX(m, r, c) ((r) >= 0 && (r) < (m).rows && (c) >= 0 && (c) < (m).cols)

#define MATRIX_GET(m, r, c) (VALID_INDEX(m, r, c) ? (m).data[r][c] : 0)
#define MATRIX_SET(m, r, c, v) do { if (VALID_INDEX(m, r, c)) (m).data[r][c] = (v); } while(0)

#define SAFE_ADD(a, b) (((b) > 0 && (a) > INT_MAX - (b)) || ((b) < 0 && (a) < INT_MIN - (b))) ? 0 : ((a) + (b))
#define SAFE_MULT(a, b) (((a) > 0 && (b) > 0 && (a) > INT_MAX / (b)) || ((a) > 0 && (b) < 0 && (b) < INT_MIN / (a)) || ((a) < 0 && (b) > 0 && (a) < INT_MIN / (b)) || ((a) < 0 && (b) < 0 && (a) < INT_MAX / (b))) ? 0 : ((a) * (b))

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    long value;
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || value < min || value > max) {
        return -1;
    }
    
    return (int)value;
}

void matrix_init(Matrix* m) {
    m->rows = 0;
    m->cols = 0;
    memset(m->data, 0, sizeof(m->data));
}

int matrix_input(Matrix* m) {
    int rows, cols;
    
    rows = read_int("Enter number of rows (1-10): ", 1, MAX_ROWS);
    if (rows == -1) return 0;
    
    cols = read_int("Enter number of columns (1-10): ", 1, MAX_COLS);
    if (cols == -1) return 0;
    
    if (!VALID_DIMENSIONS(rows, cols)) return 0;
    
    m->rows = rows;
    m->cols = cols;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val = read_int("", INT_MIN, INT_MAX);
            if (val == -1) return 0;
            MATRIX_SET(*m, i, j, val);
        }
    }
    
    return 1;
}

void matrix_print(const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", MATRIX_GET(*m, i, j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) return 0;
    if (!VALID_DIMENSIONS(a->rows, b->cols)) return 0;
    
    matrix_init(result);
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int product = SAFE_MULT(MATRIX_GET(*a, i, k), MATRIX_GET(*b, k, j));
                sum = SAFE_ADD(sum, product);
            }
            MATRIX_SET(*result, i, j, sum);
        }
    }
    
    return 1;
}

int matrix_transpose(const Matrix* m, Matrix* result) {
    if (!VALID_DIMENSIONS(m->cols, m->rows)) return 0;
    
    matrix_init(result);
    result->rows = m->cols;
    result->cols = m->rows;
    
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            MATRIX_SET(*result