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

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
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

void matrix_transpose(const Matrix* input, Matrix* result) {
    result->rows = input->cols;
    result->cols = input->rows;
    
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            result->data[j][i] = input->data[i][j];
        }
    }
}

double matrix_trace(const Matrix* m) {
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

void print_matrix(const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%8.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char* prompt, int min, int max) {
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
        printf("Please enter a valid integer between %d and %d\n", min, max);
    }
}

void read_matrix(Matrix* m) {
    m->rows = read_int("Enter number of rows (1-10): ", 1, 10);
    m->cols = read_int("Enter number of columns (1-10): ", 1, 10);
    
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            char buffer[100];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                exit(1);
            }
            if (sscanf(buffer, "%lf", &m->data[i][j]) != 1) {
                fprintf(stderr, "Invalid number format\n");
                exit(1);
            }
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    srand((unsigned int)time(NULL));
    
    printf("Matrix Operations Program\n");
    printf("1. Matrix Multiplication\n");
    printf("2. Matrix Transpose\n");
    printf("3. Matrix Trace\n");
    printf("4. Exit\n");
    
    while (1) {
        choice = read_int("Choose operation (1-4): ", 1, 4);
        
        if (choice == 4) {
            break;
        }
        
        switch (choice) {
            case 1:
                printf("Enter first matrix:\n");
                read_matrix(&a);
                printf("Enter second matrix:\n");
                read_matrix(&b);
                matrix_multiply(&a, &b, &result);
                printf("Result of multiplication:\n");
                print_matrix(&result);
                break;
                
            case 2:
                printf("Enter matrix:\n");
                read_matrix(&a);
                matrix_transpose(&a, &result);
                printf("Transpose:\n");
                print_matrix(&result);
                break;
                
            case 3:
                printf("Enter square matrix:\n");
                read_matrix(&a);
                if (a.rows != a.cols) {
                    printf("Error: Matrix must be square for trace\n");
                    break;
                }
                printf("Trace: %.2f\n", matrix_trace(&a));
                break;
        }
        
        printf("\n");
    }
    
    return 0;
}