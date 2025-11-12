//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_ELEM 1000
#define MIN_ELEM -1000

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

void init_matrix(Matrix *m) {
    m->rows = 0;
    m->cols = 0;
    memset(m->data, 0, sizeof(m->data));
}

int read_int(const char *prompt, int min_val, int max_val) {
    char buffer[32];
    long val;
    char *endptr;

    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (val < min_val || val > max_val) {
        return -1;
    }
    return (int)val;
}

void input_matrix(Matrix *m) {
    int r, c;

    m->rows = read_int("Enter number of rows (1-10): ", MIN_DIM, MAX_ROWS);
    if (m->rows == -1) {
        printf("Invalid rows input\n");
        exit(1);
    }

    m->cols = read_int("Enter number of columns (1-10): ", MIN_DIM, MAX_COLS);
    if (m->cols == -1) {
        printf("Invalid columns input\n");
        exit(1);
    }

    printf("Enter matrix elements (%d x %d):\n", m->rows, m->cols);
    for (r = 0; r < m->rows; r++) {
        for (c = 0; c < m->cols; c++) {
            int elem = read_int("", MIN_ELEM, MAX_ELEM);
            if (elem == -1) {
                printf("Invalid element input\n");
                exit(1);
            }
            m->data[r][c] = elem;
        }
    }
}

void print_matrix(const Matrix *m) {
    int r, c;
    for (r = 0; r < m->rows; r++) {
        for (c = 0; c < m->cols; c++) {
            printf("%6d", m->data[r][c]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    int i, j, k;
    int temp;

    if (a->cols != b->rows) {
        return 0;
    }

    result->rows = a->rows;
    result->cols = b->cols;

    for (i = 0; i < result->rows; i++) {
        for (j = 0; j < result->cols; j++) {
            result->data[i][j] = 0;
            for (k = 0; k < a->cols; k++) {
                temp = safe_multiply(a->data[i][k], b->data[k][j]);
                if (temp == 0 && (a->data[i][k] != 0 && b->data[k][j] != 0)) {
                    return 0;
                }
                temp = safe_add(result->data[i][j], temp);
                if (temp == 0 && result->data[i][j] != 0 && temp != result->data[i][j] + a->data[i][k] * b->data[k][j]) {
                    return 0;
                }
                result->data[i][j] = temp;
            }
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;

    init_matrix(&mat1);
    init_matrix(&mat2);
    init_matrix(&result);

    printf("Matrix 1:\n");
    input_matrix(&mat1);

    printf("Matrix 2:\n");
    input_matrix(&mat2);

    printf("\nMatrix 1:\n");
    print_matrix(&mat1);

    printf("\nMatrix 2:\n");
    print_matrix(&mat2);

    if (multiply_matrices(&mat1, &mat2, &result)) {
        printf("\nMatrix product:\n");
        print_matrix(&result);
    } else {
        printf("\nMatrix multiplication failed: dimension mismatch or integer overflow\n");
    }

    return 0;
}