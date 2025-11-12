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
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int matrix_transpose(const Matrix *src, Matrix *dest) {
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
    
    int sum = 0;
    for (int i = 0; i < mat->rows; i++) {
        sum += mat->data[i][i];
    }
    *trace = sum;
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
    Matrix mat1, mat2, result;
    int choice;
    
    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Enter first matrix:\n");
                if (!read_matrix(&mat1)) {
                    printf("Error reading first matrix.\n");
                    break;
                }
                printf("Enter second matrix:\n");
                if (!read_matrix(&mat2)) {
                    printf("Error reading second matrix.\n");
                    break;
                }
                if (matrix_multiply(&mat1, &mat2, &result)) {
                    printf("Result of multiplication:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrix multiplication not possible.\n");
                }
                break;
                
            case 2:
                printf("Enter matrix to transpose:\n");
                if (!read_matrix(&mat1)) {
                    printf("Error reading matrix.\n");
                    break;
                }
                if (matrix_transpose(&mat1, &result)) {
                    printf("Transposed matrix:\n");
                    print_matrix(&result);
                }
                break;
                
            case 3:
                printf("Enter square matrix:\n");
                if (!read_matrix(&mat1)) {
                    printf("Error reading matrix.\n");
                    break;
                }
                int trace;
                if (matrix_trace(&mat1, &trace)) {
                    printf("Matrix trace: %d\n", trace);
                } else {
                    printf("Trace only defined for square matrices.\n");
                }
                break;
                
            case 4:
                printf("Exiting program.\n");
                return 0;
                
            default:
                printf("Invalid choice. Please enter 1-4.\n");
                break;
        }
    }
    
    return 0;
}