//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1
#define MAX_INPUT_LEN 32

typedef struct {
    int data[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
} Matrix;

int safe_str_to_int(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    char* endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val > INT_MAX || val < INT_MIN) return 0;
    *result = (int)val;
    return 1;
}

int read_int(const char* prompt) {
    char input[MAX_INPUT_LEN + 1];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) return 0;
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        if (len == 0) continue;
        if (safe_str_to_int(input, &value)) return value;
        printf("Invalid input. Please enter an integer.\n");
    }
}

void init_matrix(Matrix* mat) {
    if (mat == NULL) return;
    mat->rows = 0;
    mat->cols = 0;
    memset(mat->data, 0, sizeof(mat->data));
}

int validate_matrix_dimensions(int rows, int cols) {
    return (rows >= MIN_DIM && rows <= MAX_ROWS && cols >= MIN_DIM && cols <= MAX_COLS);
}

void input_matrix(Matrix* mat) {
    if (mat == NULL) return;
    int rows, cols;
    do {
        rows = read_int("Enter number of rows (1-10): ");
        cols = read_int("Enter number of columns (1-10): ");
    } while (!validate_matrix_dimensions(rows, cols));
    mat->rows = rows;
    mat->cols = cols;
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = read_int("");
        }
    }
}

void print_matrix(const Matrix* mat) {
    if (mat == NULL) return;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return;
    if (a->rows != b->rows || a->cols != b->cols) return;
    result->rows = a->rows;
    result->cols = a->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            long sum = (long)a->data[i][j] + (long)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                result->data[i][j] = 0;
            } else {
                result->data[i][j] = (int)sum;
            }
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) return;
    if (a->cols != b->rows) return;
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long product = (long)a->data[i][k] * (long)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    sum = 0;
                    break;
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    sum = 0;
                    break;
                }
            }
            result->data[i][j] = (int)sum;
        }
    }
}

void transpose_matrix(const Matrix* mat, Matrix* result) {
    if (mat == NULL || result == NULL) return;
    result->rows = mat->cols;
    result->cols = mat->rows;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    init_matrix(&mat1);
    init_matrix(&mat2);
    init_matrix(&result);

    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n