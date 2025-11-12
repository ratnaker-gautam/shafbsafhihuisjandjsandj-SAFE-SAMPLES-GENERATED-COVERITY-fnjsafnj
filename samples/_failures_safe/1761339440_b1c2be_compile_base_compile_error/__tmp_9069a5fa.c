//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 0;
    if (rows > MAX_SIZE || cols > MAX_SIZE) return 0;
    return 1;
}

int read_matrix(Matrix *mat) {
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (scanf("%d", &mat->rows) != 1) return 0;
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (scanf("%d", &mat->cols) != 1) return 0;
    
    if (!validate_matrix_size(mat->rows, mat->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) return 0;
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MIN / b->data[k][j]) return 0;
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *src, Matrix *dest) {
    if (!validate_matrix_size(src->rows, src->cols)) return 0;
    
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
    return 1;
}

int matrix_trace(const Matrix *mat, int *trace) {
    if (mat->rows != mat->cols) return 0;
    
    *trace = 0;
    for (int i = 0; i < mat->rows; i++) {
        *trace += mat->data[i][i];
    }
    return 1;
}

void display_menu(void) {
    printf("\nMatrix Operations Menu:\n");
    printf("1. Multiply two matrices\n");
    printf("2. Transpose a matrix\n");
    printf("3. Calculate matrix trace\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main(void) {
    int choice;
    Matrix a, b, result;
    
    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Enter first matrix:\n");
                if (!read_matrix(&a)) {
                    printf("Invalid first matrix input\n");
                    break;
                }
                printf("Enter second matrix:\n");
                if (!read_matrix(&b)) {
                    printf("Invalid second matrix input\n");
                    break;
                }
                if (matrix_multiply(&a, &b, &result)) {
                    printf("Result of multiplication:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrix multiplication failed: dimension mismatch or overflow\n");
                }
                break;
                
            case 2:
                printf("Enter matrix to transpose:\n");
                if (!read_matrix(&a)) {
                    printf("Invalid matrix input\n");
                    break;
                }
                if (matrix_transpose(&a, &result)) {
                    printf("Transposed matrix:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrix transpose failed\n");
                }
                break;
                
            case 3:
                printf("Enter square matrix for trace:\n");
                if (!read_matrix(&a)) {
                    printf("Invalid matrix input\n");
                    break;
                }
                int trace;
                if (matrix_trace