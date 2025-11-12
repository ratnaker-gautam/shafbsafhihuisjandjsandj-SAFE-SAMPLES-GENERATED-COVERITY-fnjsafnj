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
        if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
        if (safe_str_to_int(input, &value)) return value;
        printf("Invalid input. Please enter an integer.\n");
    }
}

void init_matrix(Matrix* mat) {
    if (mat == NULL) return;
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            mat->data[i][j] = 0;
        }
    }
    mat->rows = 0;
    mat->cols = 0;
}

int input_matrix(Matrix* mat) {
    if (mat == NULL) return 0;
    mat->rows = read_int("Enter number of rows: ");
    if (mat->rows < MIN_DIM || mat->rows > MAX_ROWS) return 0;
    mat->cols = read_int("Enter number of columns: ");
    if (mat->cols < MIN_DIM || mat->cols > MAX_COLS) return 0;
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j] = read_int("");
        }
    }
    return 1;
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

#define ADD_MATRICES(a, b, result) \
    do { \
        if ((a)->rows != (b)->rows || (a)->cols != (b)->cols) { \
            printf("Matrix dimensions do not match for addition.\n"); \
            return; \
        } \
        (result)->rows = (a)->rows; \
        (result)->cols = (a)->cols; \
        for (int i = 0; i < (a)->rows; i++) { \
            for (int j = 0; j < (a)->cols; j++) { \
                (result)->data[i][j] = (a)->data[i][j] + (b)->data[i][j]; \
            } \
        } \
    } while(0)

#define MULTIPLY_MATRICES(a, b, result) \
    do { \
        if ((a)->cols != (b)->rows) { \
            printf("Matrix dimensions incompatible for multiplication.\n"); \
            return; \
        } \
        (result)->rows = (a)->rows; \
        (result)->cols = (b)->cols; \
        for (int i = 0; i < (a)->rows; i++) { \
            for (int j = 0; j < (b)->cols; j++) { \
                int sum = 0; \
                for (int k = 0; k < (a)->cols; k++) { \
                    sum += (a)->data[i][k] * (b)->data[k][j]; \
                } \
                (result)->data[i][j] = sum; \
            } \
        } \
    } while(0)

#define TRANSPOSE_MATRIX(a, result) \
    do { \
        (result)->rows = (a)->cols; \
        (result)->cols = (a)->rows; \
        for (int i = 0; i < (a)->rows; i++) { \
            for (int j = 0; j < (a)->cols; j++) { \
                (result)->data[j][i] = (a)->data[i][j]; \
            } \
        } \
    } while(0)

int main() {
    Matrix mat1, mat2, result;
    init_matrix(&mat1);
    init_matrix(&mat2);
    init_matrix(&result);
    
    printf("Matrix Operations Program\n");
    printf("Enter first matrix:\n");
    if (!input_matrix(&mat1)) {
        printf("Invalid matrix input.\n");
        return 1;
    }
    
    printf("Enter second matrix:\n");
    if (!input_matrix(&mat2