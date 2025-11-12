//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_ROWS][MAX_COLS];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_ROWS && cols > 0 && cols <= MAX_COLS);
}

int read_matrix(Matrix *mat) {
    printf("Enter number of rows and columns: ");
    if (scanf("%d %d", &mat->rows, &mat->cols) != 2) {
        return 0;
    }
    
    if (!validate_matrix_size(mat->rows, mat->cols)) {
        return 0;
    }
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* matrix_multiply(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) {
        return NULL;
    }
    
    Matrix *result = malloc(sizeof(Matrix));
    if (!result) {
        return NULL;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > INT_MAX - sum) ||
                    (a->data[i][k] < 0 && b->data[k][j] < INT_MIN - sum)) {
                    free(result);
                    return NULL;
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    
    return result;
}

Matrix* matrix_transpose(const Matrix *mat) {
    Matrix *result = malloc(sizeof(Matrix));
    if (!result) {
        return NULL;
    }
    
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = mat->data[j][i];
        }
    }
    
    return result;
}

int main() {
    Matrix mat1, mat2;
    
    printf("Matrix Operations Program\n");
    printf("First matrix:\n");
    if (!read_matrix(&mat1)) {
        printf("Error: Invalid first matrix input\n");
        return 1;
    }
    
    printf("Second matrix:\n");
    if (!read_matrix(&mat2)) {
        printf("Error: Invalid second matrix input\n");
        return 1;
    }
    
    printf("\nFirst matrix:\n");
    print_matrix(&mat1);
    
    printf("\nSecond matrix:\n");
    print_matrix(&mat2);
    
    Matrix *mult_result = matrix_multiply(&mat1, &mat2);
    if (mult_result) {
        printf("\nMatrix multiplication result:\n");
        print_matrix(mult_result);
        free(mult_result);
    } else {
        printf("\nMatrix multiplication failed: dimension mismatch or integer overflow\n");
    }
    
    Matrix *trans_result = matrix_transpose(&mat1);
    if (trans_result) {
        printf("\nFirst matrix transpose:\n");
        print_matrix(trans_result);
        free(trans_result);
    } else {
        printf("\nMatrix transpose failed\n");
    }
    
    trans_result = matrix_transpose(&mat2);
    if (trans_result) {
        printf("\nSecond matrix transpose:\n");
        print_matrix(trans_result);
        free(trans_result);
    } else {
        printf("\nMatrix transpose failed\n");
    }
    
    return 0;
}