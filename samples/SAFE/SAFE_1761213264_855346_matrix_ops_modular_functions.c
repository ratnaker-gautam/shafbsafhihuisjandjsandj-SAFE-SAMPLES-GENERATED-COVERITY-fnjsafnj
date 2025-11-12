//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM][MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_int_safe(const char* prompt, int* value) {
    char buffer[32];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char* endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    *value = (int)val;
    return 1;
}

int read_matrix(Matrix* mat) {
    if (!read_int_safe("Enter number of rows: ", &mat->rows)) {
        return 0;
    }
    if (!read_int_safe("Enter number of columns: ", &mat->cols)) {
        return 0;
    }
    if (!validate_dimension(mat->rows) || !validate_dimension(mat->cols)) {
        return 0;
    }
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (!read_int_safe("", &mat->data[i][j])) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%d ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((a->data[i][k] > 0 && b->data[k][j] > INT_MAX - sum) ||
                    (a->data[i][k] < 0 && b->data[k][j] < INT_MIN - sum)) {
                    return 0;
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = input->data[j][i];
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    
    if (!read_int_safe("Enter choice (1-2): ", &choice)) {
        printf("Invalid choice input\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter first matrix:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid first matrix input\n");
                return 1;
            }
            printf("Enter second matrix:\n");
            if (!read_matrix(&mat2)) {
                printf("Invalid second matrix input\n");
                return 1;
            }
            if (!multiply_matrices(&mat1, &mat2, &result)) {
                printf("Matrix multiplication failed: dimension mismatch or overflow\n");
                return 1;
            }
            printf("Result of multiplication:\n");
            print_matrix(&result);
            break;
            
        case 2:
            printf("Enter matrix to transpose:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid matrix input\n");
                return 1;
            }
            if (!transpose_matrix(&mat1, &result)) {
                printf("Matrix transpose failed\n");
                return 1;
            }
            printf("Transposed matrix:\n");
            print_matrix(&result);
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}