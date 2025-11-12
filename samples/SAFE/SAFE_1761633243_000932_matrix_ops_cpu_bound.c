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
    mat->rows = rows;
    mat->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = (double)((i + j) % 10);
        }
    }
}

void print_matrix(const Matrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6.2f ", mat->data[i][j]);
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (!validate_matrix_size(a->rows, b->cols)) {
        return 0;
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
    return 1;
}

double matrix_trace(const Matrix *mat) {
    if (mat->rows != mat->cols) {
        return 0.0;
    }
    double trace = 0.0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

int get_valid_int(const char *prompt, int min, int max) {
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
    
    int rows1 = get_valid_int("Enter rows for matrix A (1-10): ", 1, 10);
    int cols1 = get_valid_int("Enter columns for matrix A (1-10): ", 1, 10);
    int rows2 = get_valid_int("Enter rows for matrix B (1-10): ", 1, 10);
    int cols2 = get_valid_int("Enter columns for matrix B (1-10): ", 1, 10);
    
    if (rows1 == -1 || cols1 == -1 || rows2 == -1 || cols2 == -1) {
        printf("Input error.\n");
        return 1;
    }
    
    Matrix matA, matB, matResult;
    
    initialize_matrix(&matA, rows1, cols1);
    initialize_matrix(&matB, rows2, cols2);
    
    printf("\nMatrix A (%dx%d):\n", matA.rows, matA.cols);
    print_matrix(&matA);
    
    printf("\nMatrix B (%dx%d):\n", matB.rows, matB.cols);
    print_matrix(&matB);
    
    if (matrix_multiply(&matA, &matB, &matResult)) {
        printf("\nMatrix A * B (%dx%d):\n", matResult.rows, matResult.cols);
        print_matrix(&matResult);
        
        double trace = matrix_trace(&matResult);
        if (matResult.rows == matResult.cols) {
            printf("\nTrace of result matrix: %.2f\n", trace);
        } else {
            printf("\nResult matrix is not square, cannot compute trace.\n");
        }
    } else {
        printf("\nMatrix multiplication not possible (dimension mismatch or size limit).\n");
    }
    
    printf("\nPerforming additional CPU-intensive operations...\n");
    
    Matrix temp;
    int operations = 1000;
    double total = 0.0;
    
    for (int op = 0; op < operations; op++) {
        if (matrix_multiply(&matA, &matB, &temp)) {
            total += matrix_trace(&temp);
        }
    }
    
    printf("Completed %d matrix operations. Final value: %.2f\n", operations, total);
    
    return 0;
}