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
            m->data[i][j] = (double)(i * m->cols + j + 1);
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Error: Matrix dimensions incompatible for multiplication\n");
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

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Please enter an integer between %d and %d\n", min, max);
    }
}

int main() {
    Matrix a, b, result;
    
    printf("Matrix Multiplication Program\n");
    printf("=============================\n\n");
    
    a.rows = read_int("Enter rows for matrix A (1-100): ", 1, MAX_SIZE);
    a.cols = read_int("Enter columns for matrix A (1-100): ", 1, MAX_SIZE);
    
    if (!validate_matrix_size(a.rows, a.cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    b.rows = read_int("Enter rows for matrix B (1-100): ", 1, MAX_SIZE);
    b.cols = read_int("Enter columns for matrix B (1-100): ", 1, MAX_SIZE);
    
    if (!validate_matrix_size(b.rows, b.cols)) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        return 1;
    }
    
    if (a.cols != b.rows) {
        fprintf(stderr, "Error: Columns of A must equal rows of B for multiplication\n");
        return 1;
    }
    
    initialize_matrix(&a);
    initialize_matrix(&b);
    
    printf("\nMatrix A (%dx%d):\n", a.rows, a.cols);
    print_matrix(&a);
    
    printf("\nMatrix B (%dx%d):\n", b.rows, b.cols);
    print_matrix(&b);
    
    clock_t start = clock();
    matrix_multiply(&a, &b, &result);
    clock_t end = clock();
    
    printf("\nResult Matrix (%dx%d):\n", result.rows, result.cols);
    print_matrix(&result);
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nComputation time: %.6f seconds\n", cpu_time);
    
    return 0;
}