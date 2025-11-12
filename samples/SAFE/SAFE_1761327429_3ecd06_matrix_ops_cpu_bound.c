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

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

void matrix_transpose(const Matrix *input, Matrix *result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double matrix_trace(const Matrix *m) {
    if (m->rows != m->cols) {
        fprintf(stderr, "Matrix must be square for trace calculation\n");
        exit(1);
    }
    
    double trace = 0.0;
    for (int i = 0; i < m->rows; i++) {
        trace += m->data[i][i];
    }
    return trace;
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

void initialize_random_matrix(Matrix *m, int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    
    m->rows = rows;
    m->cols = cols;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = (double)(rand() % 100) / 10.0;
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    Matrix a, b, product, transpose;
    int size;
    
    printf("Enter matrix size (1-%d): ", MAX_SIZE);
    if (scanf("%d", &size) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (size <= 0 || size > MAX_SIZE) {
        fprintf(stderr, "Size must be between 1 and %d\n", MAX_SIZE);
        return 1;
    }
    
    printf("\nInitializing random %dx%d matrices...\n", size, size);
    initialize_random_matrix(&a, size, size);
    initialize_random_matrix(&b, size, size);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nComputing A * B...\n");
    matrix_multiply(&a, &b, &product);
    print_matrix(&product);
    
    printf("\nComputing trace of product matrix: %.2f\n", matrix_trace(&product));
    
    printf("\nComputing transpose of product matrix...\n");
    matrix_transpose(&product, &transpose);
    print_matrix(&transpose);
    
    printf("\nComputing trace of transposed matrix: %.2f\n", matrix_trace(&transpose));
    
    return 0;
}