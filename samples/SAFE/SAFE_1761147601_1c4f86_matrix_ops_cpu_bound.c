//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

void matrix_init(Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(1);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = 0;
        }
    }
}

void matrix_random_fill(Matrix *m, int min_val, int max_val) {
    if (min_val > max_val) {
        fprintf(stderr, "Invalid value range\n");
        exit(1);
    }
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            int range = max_val - min_val + 1;
            if (range <= 0) {
                fprintf(stderr, "Value range overflow\n");
                exit(1);
            }
            m->data[i][j] = min_val + (rand() % range);
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        fprintf(stderr, "Matrix dimensions incompatible for multiplication\n");
        exit(1);
    }
    if (result->rows != a->rows || result->cols != b->cols) {
        fprintf(stderr, "Result matrix has wrong dimensions\n");
        exit(1);
    }
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if (__builtin_mul_overflow(a->data[i][k], b->data[k][j], &sum)) {
                    fprintf(stderr, "Integer overflow in multiplication\n");
                    exit(1);
                }
                if (__builtin_add_overflow(result->data[i][j], sum, &result->data[i][j])) {
                    fprintf(stderr, "Integer overflow in addition\n");
                    exit(1);
                }
            }
        }
    }
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6d", m->data[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    Matrix a, b, result;
    
    int rows1, cols1, rows2, cols2;
    printf("Enter dimensions for first matrix (rows cols): ");
    if (scanf("%d %d", &rows1, &cols1) != 2) {
        fprintf(stderr, "Invalid input for matrix dimensions\n");
        return 1;
    }
    
    printf("Enter dimensions for second matrix (rows cols): ");
    if (scanf("%d %d", &rows2, &cols2) != 2) {
        fprintf(stderr, "Invalid input for matrix dimensions\n");
        return 1;
    }
    
    matrix_init(&a, rows1, cols1);
    matrix_init(&b, rows2, cols2);
    matrix_init(&result, rows1, cols2);
    
    matrix_random_fill(&a, 1, 10);
    matrix_random_fill(&b, 1, 10);
    
    printf("\nMatrix A:\n");
    matrix_print(&a);
    
    printf("\nMatrix B:\n");
    matrix_print(&b);
    
    matrix_multiply(&a, &b, &result);
    
    printf("\nResult (A * B):\n");
    matrix_print(&result);
    
    return 0;
}