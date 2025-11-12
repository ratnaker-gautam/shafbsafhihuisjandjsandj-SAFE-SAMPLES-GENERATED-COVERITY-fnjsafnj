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

int validate_dimension(int dim) {
    return dim > 0 && dim <= MAX_DIM;
}

int read_matrix(Matrix *mat) {
    if (mat == NULL) return 0;
    
    printf("Enter number of rows (1-%d): ", MAX_DIM);
    if (scanf("%d", &mat->rows) != 1) return 0;
    if (!validate_dimension(mat->rows)) return 0;
    
    printf("Enter number of columns (1-%d): ", MAX_DIM);
    if (scanf("%d", &mat->cols) != 1) return 0;
    if (!validate_dimension(mat->cols)) return 0;
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (scanf("%d", &mat->data[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    if (mat == NULL) return;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", mat->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->cols != b->rows) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            int64_t sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int64_t product = (int64_t)a->data[i][k] * (int64_t)b->data[k][j];
                if (product > INT_MAX || product < INT_MIN) return 0;
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) return 0;
            }
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a == NULL || b == NULL || result == NULL) return 0;
    if (a->rows != b->rows || a->cols != b->cols) return 0;
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int64_t sum = (int64_t)a->data[i][j] + (int64_t)b->data[i][j];
            if (sum > INT_MAX || sum < INT_MIN) return 0;
            result->data[i][j] = (int)sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *mat, Matrix *result) {
    if (mat == NULL || result == NULL) return 0;
    
    result->rows = mat->cols;
    result->cols = mat->rows;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
    return 1;
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
    Matrix mat1, mat2, result;
    int choice;
    
    while (1) {
        printf("\nMatrix Operations Menu:\n");
        printf("1. Matrix Multiplication\n");
        printf("2. Matrix Addition\n");
        printf("3. Matrix Transpose\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        if (choice == 4) break;
        
        switch (choice) {
            case 1:
                printf("Enter first matrix:\n");
                if (!read_matrix(&mat1)) {
                    printf("Error reading first matrix.\n");
                    clear_input_buffer();
                    break;
                }
                printf("Enter second matrix:\n");
                if (!read_matrix(&mat2)) {
                    printf("Error reading second matrix.\n");
                    clear_input_buffer();
                    break;
                }
                if (multiply_matrices(&mat1, &mat2, &result)) {
                    printf("Result of multiplication:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrix multiplication failed: dimension mismatch or integer overflow.\n");
                }
                break;
                
            case