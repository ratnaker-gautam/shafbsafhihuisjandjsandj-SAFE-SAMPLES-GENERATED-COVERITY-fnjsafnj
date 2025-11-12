//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 100
#define MAX_COLS 100

struct Matrix {
    int rows;
    int cols;
    int data[MAX_ROWS][MAX_COLS];
};

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_ROWS || cols > MAX_COLS) return 0;
    return 1;
}

void read_matrix(struct Matrix *mat) {
    printf("Enter number of rows: ");
    if (scanf("%d", &mat->rows) != 1) exit(1);
    printf("Enter number of columns: ");
    if (scanf("%d", &mat->cols) != 1) exit(1);
    
    if (!validate_matrix_dimensions(mat->rows, mat->cols)) {
        printf("Invalid matrix dimensions\n");
        exit(1);
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) exit(1);
        }
    }
}

void print_matrix(const struct Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

struct Matrix* multiply_matrices(const struct Matrix *a, const struct Matrix *b) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return NULL;
    }
    
    struct Matrix *result = malloc(sizeof(struct Matrix));
    if (!result) exit(1);
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > 0 && a->data[i][k] > INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] < 0 && a->data[i][k] < INT_MAX / b->data[k][j]) ||
                    (a->data[i][k] > 0 && b->data[k][j] < 0 && a->data[i][k] > INT_MIN / b->data[k][j]) ||
                    (a->data[i][k] < 0 && b->data[k][j] > 0 && a->data[i][k] < INT_MIN / b->data[k][j])) {
                    printf("Integer overflow detected\n");
                    free(result);
                    return NULL;
                }
                int product = a->data[i][k] * b->data[k][j];
                if ((product > 0 && sum > INT_MAX - product) || (product < 0 && sum < INT_MIN - product)) {
                    printf("Integer overflow detected\n");
                    free(result);
                    return NULL;
                }
                sum += product;
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

int main() {
    struct Matrix mat1, mat2;
    
    printf("Matrix 1:\n");
    read_matrix(&mat1);
    
    printf("Matrix 2:\n");
    read_matrix(&mat2);
    
    printf("\nMatrix 1:\n");
    print_matrix(&mat1);
    
    printf("\nMatrix 2:\n");
    print_matrix(&mat2);
    
    struct Matrix *result = multiply_matrices(&mat1, &mat2);
    if (result) {
        printf("\nResult of multiplication:\n");
        print_matrix(result);
        free(result);
    } else {
        printf("Matrix multiplication failed\n");
    }
    
    return 0;
}