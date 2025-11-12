//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_matrix_dimensions(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        return 0;
    }
    if (rows > MAX_DIM || cols > MAX_DIM) {
        return 0;
    }
    return 1;
}

int read_matrix(Matrix *mat) {
    int rows, cols;
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &rows, &cols) != 2) {
        return 0;
    }
    if (!validate_matrix_dimensions(rows, cols)) {
        return 0;
    }
    mat->rows = rows;
    mat->cols = cols;
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!validate_matrix_dimensions(a->rows, b->cols)) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    if (!validate_matrix_dimensions(a->rows, a->cols)) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = a->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *a, Matrix *result) {
    if (!validate_matrix_dimensions(a->cols, a->rows)) {
        return 0;
    }
    result->rows = a->cols;
    result->cols = a->rows;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
    return 1;
}

void display_menu(void) {
    printf("\nMatrix Operations Menu:\n");
    printf("1. Add matrices\n");
    printf("2. Multiply matrices\n");
    printf("3. Transpose matrix\n");
    printf("4. Exit\n");
    printf("Enter choice: ");
}

int main(void) {
    Matrix mat1, mat2, result;
    int choice;
    
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
                if (!read_matrix(&mat1)) {
                    printf("Invalid matrix input\n");
                    break;
                }
                printf("Enter second matrix:\n");
                if (!read_matrix(&mat2)) {
                    printf("Invalid matrix input\n");
                    break;
                }
                if (add_matrices(&mat1, &mat2, &result)) {
                    printf("Result:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrix addition failed: dimension mismatch\n");
                }
                break;
                
            case 2:
                printf("Enter first matrix:\n");
                if (!read_matrix(&mat1)) {
                    printf("Invalid matrix input\n");
                    break;
                }
                printf("Enter second matrix:\n");
                if (!read_matrix(&mat2)) {
                    printf("Invalid matrix input\n");
                    break;
                }
                if (multiply_matrices(&mat1, &mat2, &result)) {
                    printf("Result:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrix multiplication failed: dimension mismatch\n");
                }
                break;
                
            case 3:
                printf("Enter matrix:\n");