//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
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

int read_int(const char* prompt, int min_val, int max_val) {
    char input[MAX_INPUT_LEN + 2];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) return min_val;
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
        if (len >= sizeof(input) - 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Input too long\n");
            continue;
        }
        if (safe_str_to_int(input, &value) && value >= min_val && value <= max_val) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d\n", min_val, max_val);
    }
}

void init_matrix(Matrix* mat) {
    if (mat == NULL) return;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            mat->data[i][j] = read_int("", INT_MIN, INT_MAX);
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
            if ((a->data[i][j] > 0 && b->data[i][j] > INT_MAX - a->data[i][j]) ||
                (a->data[i][j] < 0 && b->data[i][j] < INT_MIN - a->data[i][j])) {
                result->data[i][j] = 0;
            } else {
                result->data[i][j] = a->data[i][j] + b->data[i][j];
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
                if (product > INT_MAX - sum || product < INT_MIN - sum) {
                    sum = 0;
                    break;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                result->data[i][j] = 0;
            } else {
                result->data[i][j] = (int)sum;
            }
        }
    }
}

void transpose_matrix(const Matrix* src, Matrix* dest) {
    if (src == NULL || dest == NULL) return;
    dest->rows = src->cols;
    dest->cols = src->rows;
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    printf("Matrix 1 dimensions:\n");
    mat1.rows = read_int("Rows: ", MIN_DIM, MAX_ROWS);
    mat1.cols = read_int("Columns: ", MIN_DIM, MAX_COLS);
    printf("Enter