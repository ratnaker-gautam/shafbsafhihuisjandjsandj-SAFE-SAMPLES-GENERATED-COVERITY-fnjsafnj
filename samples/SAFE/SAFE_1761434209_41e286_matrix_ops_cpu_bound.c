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
            mat->data[i][j] = (double)(i * cols + j + 1);
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

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%8.2f", mat->data[i][j]);
        }
        printf("\n");
    }
}

int get_positive_int(const char *prompt, int max_value) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1 && value > 0 && value <= max_value) {
            return value;
        }
        printf("Please enter a positive integer between 1 and %d\n", max_value);
    }
}

int main(void) {
    Matrix a, b, result;
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("Maximum matrix size: %dx%d\n\n", MAX_SIZE, MAX_SIZE);
    
    printf("First matrix dimensions:\n");
    rows1 = get_positive_int("Rows: ", MAX_SIZE);
    cols1 = get_positive_int("Columns: ", MAX_SIZE);
    
    printf("\nSecond matrix dimensions:\n");
    rows2 = get_positive_int("Rows: ", MAX_SIZE);
    cols2 = get_positive_int("Columns: ", MAX_SIZE);
    
    if (cols1 != rows2) {
        fprintf(stderr, "Error: Columns of first matrix must equal rows of second matrix\n");
        return 1;
    }
    
    initialize_matrix(&a, rows1, cols1);
    initialize_matrix(&b, rows2, cols2);
    
    printf("\nFirst matrix:\n");
    print_matrix(&a);
    
    printf("\nSecond matrix:\n");
    print_matrix(&b);
    
    clock_t start = clock();
    matrix_multiply(&a, &b, &result);
    clock_t end = clock();
    
    printf("\nResult matrix:\n");
    print_matrix(&result);
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nComputation time: %.6f seconds\n", cpu_time);
    
    return 0;
}