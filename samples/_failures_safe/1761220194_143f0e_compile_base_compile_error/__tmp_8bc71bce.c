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
            printf("%8.2f", mat->data[i][j]);
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

double compute_determinant(const Matrix *mat) {
    if (mat->rows != mat->cols || mat->rows > 3) {
        return 0.0;
    }
    
    if (mat->rows == 1) {
        return mat->data[0][0];
    }
    
    if (mat->rows == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    if (mat->rows == 3) {
        return mat->data[0][0] * (mat->data[1][1] * mat->data[2][2] - mat->data[1][2] * mat->data[2][1]) -
               mat->data[0][1] * (mat->data[1][0] * mat->data[2][2] - mat->data[1][2] * mat->data[2][0]) +
               mat->data[0][2] * (mat->data[1][0] * mat->data[2][1] - mat->data[1][1] * mat->data[2][0]);
    }
    
    return 0.0;
}

int get_valid_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    return value;
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    printf("Matrix Operations Demo\n");
    printf("=====================\n\n");
    
    int rows1 = get_valid_int("Enter rows for matrix A (1-5): ", 1, 5);
    int cols1 = get_valid_int("Enter columns for matrix A (1-5): ", 1, 5);
    int rows2 = get_valid_int("Enter rows for matrix B (1-5): ", 1, 5);
    int cols2 = get_valid_int("Enter columns for matrix B (1-5): ", 1, 5);
    
    Matrix matA, matB, result;
    
    initialize_matrix(&matA, rows1, cols1);
    initialize_matrix(&matB, rows2, cols2);
    
    printf("\nMatrix A (%dx%d):\n", matA.rows, matA.cols);
    print_matrix(&matA);
    
    printf("\nMatrix B (%dx%d):\n", matB.rows, matB.cols);
    print_matrix(&matB);
    
    printf("\nMatrix Multiplication:\n");
    if (multiply_matrices(&matA, &matB, &result)) {
        printf("Result (%dx%d):\n", result.rows, result.cols);
        print_matrix(&result);
    } else {
        printf("Cannot multiply: incompatible dimensions.\n");
    }
    
    printf("\nDeterminants:\n");
    if (matA.rows == matA.cols && matA.rows <= 3) {
        double detA = compute