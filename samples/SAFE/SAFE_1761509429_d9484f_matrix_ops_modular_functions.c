//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
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

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
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

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    result->rows = a->rows;
    result->cols = b->cols;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *a, Matrix *result) {
    result->rows = a->cols;
    result->cols = a->rows;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[j][i] = a->data[i][j];
        }
    }
    return 1;
}

void display_menu() {
    printf("\nMatrix Operations Menu:\n");
    printf("1. Add matrices\n");
    printf("2. Multiply matrices\n");
    printf("3. Transpose matrix\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int main() {
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
                    printf("Invalid first matrix\n");
                    break;
                }
                printf("Enter second matrix:\n");
                if (!read_matrix(&mat2)) {
                    printf("Invalid second matrix\n");
                    break;
                }
                if (add_matrices(&mat1, &mat2, &result)) {
                    printf("Result of addition:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrices cannot be added\n");
                }
                break;
                
            case 2:
                printf("Enter first matrix:\n");
                if (!read_matrix(&mat1)) {
                    printf("Invalid first matrix\n");
                    break;
                }
                printf("Enter second matrix:\n");
                if (!read_matrix(&mat2)) {
                    printf("Invalid second matrix\n");
                    break;
                }
                if (multiply_matrices(&mat1, &mat2, &result)) {
                    printf("Result of multiplication:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrices cannot be multiplied\n");
                }
                break;
                
            case 3:
                printf("Enter matrix:\n");
                if (!read_matrix(&mat1)) {
                    printf("Invalid matrix\n");
                    break;
                }
                if (transpose_matrix(&mat1, &result)) {
                    printf("Transpose:\n");
                    print_matrix(&result);
                }
                break;
                
            case 4:
                return 0;
                
            default:
                printf("Invalid choice\n");
                break;
        }
    }
    
    return 0;
}