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

void initialize_matrix(Matrix *m, int rows, int cols) {
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = (double)(i * cols + j + 1);
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication\n");
        return;
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

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int get_positive_int(const char *prompt, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value > 0 && value <= max_val) {
            return value;
        }
        printf("Please enter a positive integer between 1 and %d\n", max_val);
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("Matrix Operations Benchmark\n");
    printf("===========================\n\n");
    
    int rows1 = get_positive_int("Enter rows for matrix A (1-100): ", MAX_SIZE);
    int cols1 = get_positive_int("Enter columns for matrix A (1-100): ", MAX_SIZE);
    int rows2 = get_positive_int("Enter rows for matrix B (1-100): ", MAX_SIZE);
    int cols2 = get_positive_int("Enter columns for matrix B (1-100): ", MAX_SIZE);
    
    if (rows1 == -1 || cols1 == -1 || rows2 == -1 || cols2 == -1) {
        printf("Input error\n");
        return 1;
    }
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication\n");
        printf("Columns of A (%d) must equal rows of B (%d)\n", cols1, rows2);
        return 1;
    }
    
    Matrix a, b, result;
    
    initialize_matrix(&a, rows1, cols1);
    initialize_matrix(&b, rows2, cols2);
    
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
    
    double total_ops = 2.0 * rows1 * cols1 * cols2;
    printf("Theoretical operations: %.0f\n", total_ops);
    printf("Performance: %.2f MFLOPS\n", (total_ops / 1000000.0) / cpu_time);
    
    return 0;
}