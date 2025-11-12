//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct {
    int rows;
    int cols;
    double data[MAX_SIZE][MAX_SIZE];
} Matrix;

int validate_matrix_size(int rows, int cols) {
    return (rows > 0 && rows <= MAX_SIZE && cols > 0 && cols <= MAX_SIZE);
}

void initialize_matrix(Matrix *mat, int rows, int cols) {
    if (!validate_matrix_size(rows, cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = (double)((i + j) % 10);
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        fprintf(stderr, "Result matrix would exceed maximum size\n");
        exit(1);
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_transpose(const Matrix *input, Matrix *result) {
    if (!validate_matrix_size(input->cols, input->rows)) {
        fprintf(stderr, "Transposed matrix would exceed maximum size\n");
        exit(1);
    }
    
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double matrix_trace(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        fprintf(stderr, "Matrix must be square for trace calculation\n");
        exit(1);
    }
    
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    int size1 = 4 + (rand() % 4);
    int size2 = 4 + (rand() % 4);
    int size3 = 4 + (rand() % 4);
    
    Matrix A, B, C, D, E;
    
    printf("Matrix A (%dx%d):\n", size1, size2);
    initialize_matrix(&A, size1, size2);
    print_matrix(&A);
    
    printf("\nMatrix B (%dx%d):\n", size2, size3);
    initialize_matrix(&B, size2, size3);
    print_matrix(&B);
    
    printf("\nMatrix multiplication A * B:\n");
    matrix_multiply(&A, &B, &C);
    print_matrix(&C);
    
    printf("\nTranspose of A:\n");
    matrix_transpose(&A, &D);
    print_matrix(&D);
    
    if (size1 == size2) {
        printf("\nTrace of A: %.2f\n", matrix_trace(&A));
    }
    
    Matrix square_mat;
    int square_size = 3 + (rand() % 3);
    printf("\nSquare matrix (%dx%d):\n", square_size, square_size);
    initialize_matrix(&square_mat, square_size, square_size);
    print_matrix(&square_mat);
    printf("Trace: %.2f\n", matrix_trace(&square_mat));
    
    return 0;
}