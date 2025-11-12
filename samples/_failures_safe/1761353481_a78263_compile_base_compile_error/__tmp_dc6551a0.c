//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

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
            return -1;
        }
        
        char* endptr;
        long val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

void matrix_init(Matrix* mat, int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_DIM || cols > MAX_DIM) {
        return;
    }
    mat->rows = rows;
    mat->cols = cols;
    memset(mat->data, 0, sizeof(mat->data));
}

void matrix_print(const Matrix* mat) {
    if (mat == NULL || mat->rows <= 0 || mat->cols <= 0) {
        return;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("%6d", *(mat->data + i * mat->cols + j));
        }
        printf("\n");
    }
}

void matrix_add(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) {
        return;
    }
    if (a->rows != b->rows || a->cols != b->cols) {
        return;
    }
    
    matrix_init(result, a->rows, a->cols);
    
    int size = a->rows * a->cols;
    for (int i = 0; i < size; i++) {
        if ((*(a->data + i) > 0 && *(b->data + i) > INT_MAX - *(a->data + i)) ||
            (*(a->data + i) < 0 && *(b->data + i) < INT_MIN - *(a->data + i))) {
            return;
        }
        *(result->data + i) = *(a->data + i) + *(b->data + i);
    }
}

void matrix_multiply(const Matrix* a, const Matrix* b, Matrix* result) {
    if (a == NULL || b == NULL || result == NULL) {
        return;
    }
    if (a->cols != b->rows) {
        return;
    }
    
    matrix_init(result, a->rows, b->cols);
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                int term1 = *(a->data + i * a->cols + k);
                int term2 = *(b->data + k * b->cols + j);
                
                if (term1 != 0 && term2 != 0) {
                    if ((term1 > 0 && term2 > 0 && term1 > INT_MAX / term2) ||
                        (term1 < 0 && term2 < 0 && term1 < INT_MAX / term2) ||
                        (term1 > 0 && term2 < 0 && term2 < INT_MIN / term1) ||
                        (term1 < 0 && term2 > 0 && term1 < INT_MIN / term2)) {
                        return;
                    }
                }
                
                int product = term1 * term2;
                
                if ((sum > 0 && product > INT_MAX - sum) ||
                    (sum < 0 && product < INT_MIN - sum)) {
                    return;
                }
                
                sum += product;
            }
            *(result->data + i * result->cols + j) = sum;
        }
    }
}

void matrix_transpose(const Matrix* mat, Matrix* result) {
    if (mat == NULL || result == NULL) {
        return;
    }
    
    matrix_init(result, mat->cols, mat->rows);
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            *(result->data + j * result->cols + i) = *(mat->data + i * mat->cols + j);
        }
    }
}

void matrix_fill_random(Matrix* mat) {
    if (mat == NULL || mat->rows <= 0 || mat->cols <= 0) {
        return;
    }
    
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        *(mat->data + i) = rand() % 100;
    }
}

int main() {
    srand(42);
    
    printf("Matrix Operations Program\n");
    printf("========================