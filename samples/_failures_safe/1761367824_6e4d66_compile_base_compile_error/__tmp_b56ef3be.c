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

int read_int(const char* prompt, int min, int max) {
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
        
        if (value >= min && value <= max) {
            return value;
        }
        printf("Value must be between %d and %d.\n", min, max);
    }
}

void init_matrix(Matrix* mat) {
    printf("Initializing matrix %dx%d:\n", mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "  Element [%d][%d]: ", i, j);
            *(*(mat->data + i) + j) = read_int(prompt, -100, 100);
        }
    }
}

void print_matrix(const Matrix* mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(*(mat->data + i) + j));
        }
        printf("\n");
    }
}

void add_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Matrix dimensions don't match for addition.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = a->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            int sum = *(*(a->data + i) + j) + *(*(b->data + i) + j);
            if ((*(*(a->data + i) + j) > 0 && *(*(b->data + i) + j) > INT_MAX - *(*(a->data + i) + j)) ||
                (*(*(a->data + i) + j) < 0 && *(*(b->data + i) + j) < INT_MIN - *(*(a->data + i) + j))) {
                printf("Integer overflow detected at [%d][%d].\n", i, j);
                *(*(result->data + i) + j) = 0;
            } else {
                *(*(result->data + i) + j) = sum;
            }
        }
    }
}

void multiply_matrices(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return;
    }
    
    result->rows = a->rows;
    result->cols = b->cols;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            long long sum = 0;
            for (int k = 0; k < a->cols; k++) {
                long long product = (long long)*(*(a->data + i) + k) * (long long)*(*(b->data + k) + j);
                if (sum > 0 && product > LLONG_MAX - sum) {
                    printf("Integer overflow detected at [%d][%d].\n", i, j);
                    sum = 0;
                    break;
                }
                if (sum < 0 && product < LLONG_MIN - sum) {
                    printf("Integer overflow detected at [%d][%d].\n", i, j);
                    sum = 0;
                    break;
                }
                sum += product;
            }
            if (sum > INT_MAX || sum < INT_MIN) {
                printf("Result out of integer range at [%d][%d].\n", i, j);
                *(*(result->data + i) + j) = 0;
            } else {
                *(*(result->data + i) + j) = (int)sum;
            }
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    printf("=========================\n\n");
    
    printf("First Matrix:\n");
    mat1.rows = read_int("Enter rows (1-10): ", 1, MAX_SIZE);
    mat1.cols = read_int("Enter columns (1-10): ", 1, MAX_SIZE);
    init_matrix(&mat1);
    
    printf("\nSecond Matrix:\n");
    mat2.rows = read_int("Enter rows (1-10): ", 1, MAX_SIZE);
    mat2.cols = read_int("Enter columns (1-10): ", 1, MAX_SIZE);
    init_matrix(&mat2);
    
    printf("\nFirst