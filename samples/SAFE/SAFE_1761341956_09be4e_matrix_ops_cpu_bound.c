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
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
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

void generate_random_matrix(Matrix *m, int rows, int cols) {
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

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    Matrix a, b, c, d;
    int size = 5;
    
    printf("Generating random %dx%d matrices...\n", size, size);
    
    generate_random_matrix(&a, size, size);
    generate_random_matrix(&b, size, size);
    
    printf("\nMatrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    printf("\nComputing A × B...\n");
    matrix_multiply(&a, &b, &c);
    print_matrix(&c);
    
    printf("\nComputing trace of A × B: %.2f\n", matrix_trace(&c));
    
    printf("\nComputing transpose of A × B...\n");
    matrix_transpose(&c, &d);
    print_matrix(&d);
    
    printf("\nComputing trace of transpose: %.2f\n", matrix_trace(&d));
    
    Matrix e, f, g;
    generate_random_matrix(&e, 3, 4);
    generate_random_matrix(&f, 4, 2);
    
    printf("\nTesting non-square matrices (3x4 × 4x2):\n");
    printf("Matrix E (3x4):\n");
    print_matrix(&e);
    printf("Matrix F (4x2):\n");
    print_matrix(&f);
    
    printf("E × F:\n");
    matrix_multiply(&e, &f, &g);
    print_matrix(&g);
    
    return 0;
}