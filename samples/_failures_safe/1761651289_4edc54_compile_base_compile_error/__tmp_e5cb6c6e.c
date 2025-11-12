//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
} Matrix;

int validate_dimension(int dim) {
    return (dim > 0 && dim <= MAX_DIM);
}

int read_matrix(Matrix *mat) {
    char buffer[256];
    
    printf("Enter number of rows: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%d", &mat->rows) != 1) return 0;
    if (!validate_dimension(mat->rows)) return 0;
    
    printf("Enter number of columns: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%d", &mat->cols) != 1) return 0;
    if (!validate_dimension(mat->cols)) return 0;
    
    printf("Enter %d elements row by row:\n", mat->rows * mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
            if (sscanf(buffer, "%d", mat->data + i * mat->cols + j) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) return 0;
    if (!validate_dimension(a->rows) || !validate_dimension(b->cols)) return 0;
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int term1 = *(a->data + i * a->cols + k);
                int term2 = *(b->data + k * b->cols + j);
                if ((term1 > 0 && term2 > INT_MAX / term1) || 
                    (term1 < 0 && term2 < INT_MAX / term1)) return 0;
                int product = term1 * term2;
                if ((product > 0 && sum > INT_MAX - product) || 
                    (product < 0 && sum < INT_MIN - product)) return 0;
                sum += product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
    return 1;
}

int transpose_matrix(const Matrix *src, Matrix *dst) {
    if (!validate_dimension(src->rows) || !validate_dimension(src->cols)) return 0;
    
    dst->rows = src->cols;
    dst->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dst->data + j * dst->cols + i) = *(src->data + i * src->cols + j);
        }
    }
    return 1;
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    char buffer[256];
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf("Enter choice: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    if (sscanf(buffer, "%d", &choice) != 1) return 1;
    
    switch (choice) {
        case 1:
            printf("Enter first matrix:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid input for first matrix\n");
                return 1;
            }
            printf("Enter second matrix:\n");
            if (!read_matrix(&mat2)) {
                printf("Invalid input for second matrix\n");
                return 1;
            }
            if (!multiply_matrices(&mat1, &mat2, &result)) {
                printf("Matrix multiplication failed - dimension mismatch or overflow\n");
                return 1;
            }
            printf("Result of multiplication:\n");
            print_matrix(&result);
            break;
            
        case 2:
            printf("Enter matrix to transpose:\n");
            if (!read_matrix(&mat1)) {
                printf("Invalid input for matrix\n");
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
    
    return 0