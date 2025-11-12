//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    MATRIX_ADD,
    MATRIX_SUBTRACT,
    MATRIX_MULTIPLY,
    MATRIX_TRANSPOSE
} MatrixOperation;

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

Matrix* create_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;
    
    Matrix *matrix = malloc(sizeof(Matrix));
    if (!matrix) return NULL;
    
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(rows * sizeof(double*));
    if (!matrix->data) {
        free(matrix);
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        matrix->data[i] = calloc(cols, sizeof(double));
        if (!matrix->data[i]) {
            for (int j = 0; j < i; j++) {
                free(matrix->data[j]);
            }
            free(matrix->data);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(Matrix *matrix) {
    if (!matrix) return;
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

int read_matrix_input(Matrix *matrix) {
    if (!matrix) return 0;
    
    printf("Enter %d x %d matrix elements:\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            if (scanf("%lf", &matrix->data[i][j]) != 1) {
                return 0;
            }
        }
    }
    return 1;
}

void print_matrix(Matrix *matrix) {
    if (!matrix) return;
    
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%8.2f", matrix->data[i][j]);
        }
        printf("\n");
    }
}

Matrix* perform_operation(MatrixOperation op, Matrix *a, Matrix *b) {
    if (!a) return NULL;
    
    switch (op) {
        case MATRIX_ADD:
            if (!b || a->rows != b->rows || a->cols != b->cols) return NULL;
            {
                Matrix *result = create_matrix(a->rows, a->cols);
                if (!result) return NULL;
                for (int i = 0; i < a->rows; i++) {
                    for (int j = 0; j < a->cols; j++) {
                        result->data[i][j] = a->data[i][j] + b->data[i][j];
                    }
                }
                return result;
            }
            
        case MATRIX_SUBTRACT:
            if (!b || a->rows != b->rows || a->cols != b->cols) return NULL;
            {
                Matrix *result = create_matrix(a->rows, a->cols);
                if (!result) return NULL;
                for (int i = 0; i < a->rows; i++) {
                    for (int j = 0; j < a->cols; j++) {
                        result->data[i][j] = a->data[i][j] - b->data[i][j];
                    }
                }
                return result;
            }
            
        case MATRIX_MULTIPLY:
            if (!b || a->cols != b->rows) return NULL;
            {
                Matrix *result = create_matrix(a->rows, b->cols);
                if (!result) return NULL;
                for (int i = 0; i < a->rows; i++) {
                    for (int j = 0; j < b->cols; j++) {
                        result->data[i][j] = 0;
                        for (int k = 0; k < a->cols; k++) {
                            result->data[i][j] += a->data[i][k] * b->data[k][j];
                        }
                    }
                }
                return result;
            }
            
        case MATRIX_TRANSPOSE:
            {
                Matrix *result = create_matrix(a->cols, a->rows);
                if (!result) return NULL;
                for (int i = 0; i < a->rows; i++) {
                    for (int j = 0; j < a->cols; j++) {
                        result->data[j][i] = a->data[i][j];
                    }
                }
                return result;
            }
            
        default:
            return NULL;
    }
}

int main() {
    Matrix *matrix1 = NULL;
    Matrix *matrix2 = NULL;
    Matrix *result = NULL;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Add\n2. Subtract\n3. Multiply\n4. Transpose\n");
    printf("Enter operation (1-4): ");
    
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        printf("Invalid choice\n");
        return 1;
    }
    
    MatrixOperation op = choice - 1;
    
    if (op == MATRIX_TRANSPOSE) {
        int rows, cols;