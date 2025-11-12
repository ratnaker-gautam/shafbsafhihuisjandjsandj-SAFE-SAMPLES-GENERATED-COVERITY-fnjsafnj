//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_SIZE 8

typedef struct {
    int data[MATRIX_SIZE][MATRIX_SIZE];
} Matrix;

void matrix_init(Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            m->data[i][j] = i * MATRIX_SIZE + j;
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                if (a->data[i][k] > 0 && b->data[k][j] > 0) {
                    if (a->data[i][k] > INT_MAX / b->data[k][j]) {
                        sum = INT_MAX;
                        break;
                    }
                } else if (a->data[i][k] < 0 && b->data[k][j] < 0) {
                    if (a->data[i][k] < INT_MAX / b->data[k][j]) {
                        sum = INT_MAX;
                        break;
                    }
                }
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%6d ", m->data[i][j]);
        }
        printf("\n");
    }
}

int read_int(const char *prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

int main(void) {
    Matrix a, b, result;
    int iterations;
    
    printf("Matrix Operations Benchmark\n");
    printf("Matrix size: %dx%d\n\n", MATRIX_SIZE, MATRIX_SIZE);
    
    matrix_init(&a);
    matrix_init(&b);
    
    printf("Matrix A:\n");
    matrix_print(&a);
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    iterations = read_int("Enter number of iterations (1-1000): ", 1, 1000);
    
    printf("\nPerforming %d matrix multiplications...\n", iterations);
    
    for (int i = 0; i < iterations; i++) {
        matrix_multiply(&a, &b, &result);
        if (i == 0) {
            printf("First result matrix:\n");
            matrix_print(&result);
        }
    }
    
    printf("Completed %d iterations.\n", iterations);
    
    return 0;
}