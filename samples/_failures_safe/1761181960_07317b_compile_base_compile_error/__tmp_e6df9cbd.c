//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

void init_matrix(Matrix *m, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    m->rows = rows;
    m->cols = cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            *(*(m->data + i) + j) = 0;
        }
    }
}

void read_matrix(Matrix *m) {
    printf("Enter matrix dimensions (rows cols): ");
    if (scanf("%d %d", &m->rows, &m->cols) != 2) {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }
    if (m->rows <= 0 || m->cols <= 0 || m->rows > MAX_SIZE || m->cols > MAX_SIZE) {
        fprintf(stderr, "Invalid matrix dimensions\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter %d elements:\n", m->rows * m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (scanf("%d", *(m->data + i) + j) != 1) {
                fprintf(stderr, "Invalid input\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%d ", *(*(m->data + i) + j));
        }
        printf("\n");
    }
}

int multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->cols != b->rows) {
        return 0;
    }
    if (a->rows > MAX_SIZE || b->cols > MAX_SIZE) {
        return 0;
    }
    init_matrix(result, a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                if ((*(*(a->data + i) + k) > 0 && *(*(b->data + k) + j) > INT_MAX - sum) ||
                    (*(*(a->data + i) + k) < 0 && *(*(b->data + k) + j) < INT_MIN - sum)) {
                    return 0;
                }
                sum += *(*(a->data + i) + k) * *(*(b->data + k) + j);
            }
            *(*(result->data + i) + j) = sum;
        }
    }
    return 1;
}

int add_matrices(const Matrix *a, const Matrix *b, Matrix *result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }
    init_matrix(result, a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if ((*(*(a->data + i) + j) > 0 && *(*(b->data + i) + j) > INT_MAX - *(*(a->data + i) + j)) ||
                (*(*(a->data + i) + j) < 0 && *(*(b->data + i) + j) < INT_MIN - *(*(a->data + i) + j))) {
                return 0;
            }
            *(*(result->data + i) + j) = *(*(a->data + i) + j) + *(*(b->data + i) + j);
        }
    }
    return 1;
}

void transpose_matrix(const Matrix *m, Matrix *result) {
    init_matrix(result, m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            *(*(result->data + j) + i) = *(*(m->data + i) + j);
        }
    }
}

int main() {
    Matrix a, b, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("1. Add matrices\n");
    printf("2. Multiply matrices\n");
    printf("3. Transpose matrix\n");
    printf("Enter choice: ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    switch (choice) {
        case 1:
            printf("Matrix A:\n");
            read_matrix(&a);
            printf("Matrix B:\n");
            read_matrix(&