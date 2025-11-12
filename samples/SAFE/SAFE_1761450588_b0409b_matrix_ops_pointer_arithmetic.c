//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 16

typedef struct {
    int rows;
    int cols;
    int data[MAX_DIM * MAX_DIM];
} Matrix;

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
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
        printf("Invalid input. Enter value between %d and %d.\n", min, max);
    }
}

void matrix_init(Matrix* mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_input(Matrix* mat, const char* name) {
    printf("Enter values for matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            *(mat->data + i * mat->cols + j) = read_int("", INT_MIN, INT_MAX);
        }
    }
}

void matrix_print(const Matrix* mat, const char* name) {
    printf("Matrix %s (%dx%d):\n", name, mat->rows, mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d ", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

int matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a->cols != b->rows) {
        return 0;
    }
    
    matrix_init(result, a->rows, b->cols);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += (*(a->data + i * a->cols + k)) * (*(b->data + k * b->cols + j));
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
    return 1;
}

void matrix_transpose(const Matrix* src, Matrix* dest) {
    matrix_init(dest, src->cols, src->rows);
    
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            *(dest->data + j * dest->cols + i) = *(src->data + i * src->cols + j);
        }
    }
}

int main() {
    Matrix mat1, mat2, result;
    int choice;
    
    printf("Matrix Operations Program\n");
    
    int rows1 = read_int("Enter rows for matrix 1 (1-16): ", 1, MAX_DIM);
    int cols1 = read_int("Enter columns for matrix 1 (1-16): ", 1, MAX_DIM);
    matrix_init(&mat1, rows1, cols1);
    matrix_input(&mat1, "A");
    
    int rows2 = read_int("Enter rows for matrix 2 (1-16): ", 1, MAX_DIM);
    int cols2 = read_int("Enter columns for matrix 2 (1-16): ", 1, MAX_DIM);
    matrix_init(&mat2, rows2, cols2);
    matrix_input(&mat2, "B");
    
    while (1) {
        printf("\nMatrix Operations Menu:\n");
        printf("1. Display matrices\n");
        printf("2. Multiply matrices\n");
        printf("3. Transpose matrix A\n");
        printf("4. Transpose matrix B\n");
        printf("5. Exit\n");
        
        choice = read_int("Enter choice: ", 1, 5);
        
        switch (choice) {
            case 1:
                matrix_print(&mat1, "A");
                matrix_print(&mat2, "B");
                break;
                
            case 2:
                if (matrix_multiply(&mat1, &mat2, &result)) {
                    matrix_print(&result, "A x B");
                } else {
                    printf("Matrix multiplication not possible: columns of A must equal rows of B\n");
                }
                break;
                
            case 3:
                matrix_transpose(&mat1, &result);
                matrix_print(&result, "A^T");
                break;
                
            case 4:
                matrix_transpose(&mat2, &result);
                matrix_print(&result, "B^T");
                break;
                
            case 5:
                return 0;
        }
    }
    
    return 0;
}