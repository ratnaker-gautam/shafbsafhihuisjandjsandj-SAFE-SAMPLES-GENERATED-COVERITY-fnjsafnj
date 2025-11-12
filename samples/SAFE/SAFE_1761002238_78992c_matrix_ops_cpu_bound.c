//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MATRIX_SIZE 8

typedef struct {
    double data[MATRIX_SIZE][MATRIX_SIZE];
} Matrix;

void matrix_init_identity(Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            m->data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

void matrix_init_random(Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            m->data[i][j] = (double)rand() / RAND_MAX * 10.0 - 5.0;
        }
    }
}

void matrix_multiply(const Matrix *a, const Matrix *b, Matrix *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            double sum = 0.0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                sum += a->data[i][k] * b->data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
}

void matrix_transpose(const Matrix *m, Matrix *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
}

double matrix_determinant(const Matrix *m) {
    Matrix temp = *m;
    double det = 1.0;
    
    for (int i = 0; i < MATRIX_SIZE; i++) {
        int pivot = i;
        for (int j = i + 1; j < MATRIX_SIZE; j++) {
            if (fabs(temp.data[j][i]) > fabs(temp.data[pivot][i])) {
                pivot = j;
            }
        }
        
        if (pivot != i) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                double tmp = temp.data[i][j];
                temp.data[i][j] = temp.data[pivot][j];
                temp.data[pivot][j] = tmp;
            }
            det = -det;
        }
        
        if (fabs(temp.data[i][i]) < 1e-12) {
            return 0.0;
        }
        
        det *= temp.data[i][i];
        
        for (int j = i + 1; j < MATRIX_SIZE; j++) {
            double factor = temp.data[j][i] / temp.data[i][i];
            for (int k = i + 1; k < MATRIX_SIZE; k++) {
                temp.data[j][k] -= factor * temp.data[i][k];
            }
        }
    }
    
    return det;
}

void matrix_print(const Matrix *m) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%8.3f ", m->data[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    srand(time(NULL));
    
    Matrix a, b, c, d;
    
    printf("Matrix A (random):\n");
    matrix_init_random(&a);
    matrix_print(&a);
    
    printf("\nMatrix B (identity):\n");
    matrix_init_identity(&b);
    matrix_print(&b);
    
    printf("\nMatrix C = A × B:\n");
    matrix_multiply(&a, &b, &c);
    matrix_print(&c);
    
    printf("\nMatrix D = transpose of A:\n");
    matrix_transpose(&a, &d);
    matrix_print(&d);
    
    double det = matrix_determinant(&a);
    printf("\nDeterminant of A: %.6f\n", det);
    
    Matrix e, f;
    matrix_init_random(&e);
    matrix_init_random(&f);
    
    printf("\nMatrix E (random):\n");
    matrix_print(&e);
    printf("\nMatrix F (random):\n");
    matrix_print(&f);
    
    printf("\nMatrix G = E × F:\n");
    Matrix g;
    matrix_multiply(&e, &f, &g);
    matrix_print(&g);
    
    double det_g = matrix_determinant(&g);
    printf("\nDeterminant of G: %.6f\n", det_g);
    
    return 0;
}