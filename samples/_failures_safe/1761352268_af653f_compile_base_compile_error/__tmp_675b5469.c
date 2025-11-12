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

void initialize_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->data[i][j] = (double)((i + j) % 10);
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        return 0;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return 1;
}

void transpose_matrix(const Matrix *src, Matrix *dest) {
    dest->rows = src->cols;
    dest->cols = src->rows;
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            dest->data[j][i] = src->data[i][j];
        }
    }
}

double compute_determinant_2x2(const Matrix *m) {
    if (m->rows != 2 || m->cols != 2) {
        return 0.0;
    }
    return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
}

int get_integer_input(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("Matrix Operations Demo\n");
    printf("=====================\n\n");
    
    int rows1 = get_integer_input("Enter rows for matrix A (1-10): ", 1, 10);
    int cols1 = get_integer_input("Enter columns for matrix A (1-10): ", 1, 10);
    int rows2 = get_integer_input("Enter rows for matrix B (1-10): ", 1, 10);
    int cols2 = get_integer_input("Enter columns for matrix B (1-10): ", 1, 10);
    
    if (rows1 == -1 || cols1 == -1 || rows2 == -1 || cols2 == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    Matrix matA = {rows1, cols1};
    Matrix matB = {rows2, cols2};
    
    initialize_matrix(&matA);
    initialize_matrix(&matB);
    
    printf("\nMatrix A:\n");
    print_matrix(&matA);
    printf("\nMatrix B:\n");
    print_matrix(&matB);
    
    Matrix result;
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&matA, &matB, &result)) {
        print_matrix(&result);
    } else {
        printf("Cannot multiply: dimension mismatch or size limit exceeded.\n");
    }
    
    printf("\nTranspose of Matrix A:\n");
    Matrix transA;
    transpose_matrix(&matA, &transA);
    print_matrix(&transA);
    
    if (matA.rows == 2 && matA.cols == 2) {
        double det = compute_determinant_2x2(&matA);
        printf("\nDeterminant of Matrix A: %.2f\n", det);
    }
    
    printf("\nPerforming intensive matrix operations...\n");
    
    Matrix temp;
    double total = 0.0;
    for (int iter = 0; iter < 10000; iter++) {
        for (int i = 0; i < matA.rows; i++) {
            for (int j = 0; j <