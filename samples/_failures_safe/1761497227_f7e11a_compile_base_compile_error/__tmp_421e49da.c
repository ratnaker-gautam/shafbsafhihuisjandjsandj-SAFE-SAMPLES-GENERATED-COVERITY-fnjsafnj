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

void transpose_matrix(const Matrix *input, Matrix *result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double calculate_determinant_2x2(const Matrix *m) {
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
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min, max);
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n");
    
    int rows1 = get_integer_input("Enter rows for matrix A (1-10): ", 1, 10);
    int cols1 = get_integer_input("Enter columns for matrix A (1-10): ", 1, 10);
    initialize_matrix(&a, rows1, cols1);
    
    int rows2 = get_integer_input("Enter rows for matrix B (1-10): ", 1, 10);
    int cols2 = get_integer_input("Enter columns for matrix B (1-10): ", 1, 10);
    initialize_matrix(&b, rows2, cols2);
    
    while (1) {
        printf("\nAvailable Operations:\n");
        printf("1. Display matrices\n");
        printf("2. Matrix multiplication\n");
        printf("3. Matrix transpose\n");
        printf("4. 2x2 determinant\n");
        printf("5. Exit\n");
        
        choice = get_integer_input("Select operation (1-5): ", 1, 5);
        
        switch (choice) {
            case 1:
                printf("\nMatrix A:\n");
                print_matrix(&a);
                printf("\nMatrix B:\n");
                print_matrix(&b);
                break;
                
            case 2:
                if (multiply_matrices(&a, &b, &result)) {
                    printf("\nMatrix A * B:\n");
                    print_matrix(&result);
                } else {
                    printf("Matrix multiplication not possible - dimension mismatch.\n");
                }
                break;
                
            case 3:
                printf("\nTranspose of A:\n");
                transpose_matrix(&a, &result);
                print_matrix(&result);
                break;
                
            case 4:
                if (a.rows == 2 && a.cols == 2) {
                    double det = calculate_determinant_2x2(&a);
                    printf("Determinant of matrix A: %.2f\n", det);
                } else {
                    printf("Determinant calculation only available for 2x2 matrices.\n");
                }