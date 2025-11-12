//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_MAX_SIZE 10
#define MATRIX_MIN_SIZE 1
#define MATRIX_ELEM_MAX 1000
#define MATRIX_ELEM_MIN -1000

#define MATRIX_GET(matrix, rows, cols, i, j) ((i) < (rows) && (j) < (cols) ? (matrix)[(i) * (cols) + (j)] : 0)
#define MATRIX_SET(matrix, rows, cols, i, j, val) do { \
    if ((i) < (rows) && (j) < (cols)) { \
        (matrix)[(i) * (cols) + (j)] = (val); \
    } \
} while(0)

#define MATRIX_ALLOC(rows, cols) (malloc((rows) * (cols) * sizeof(int)))
#define MATRIX_FREE(matrix) free(matrix)

#define MATRIX_PRINT(matrix, rows, cols) do { \
    for (int i = 0; i < (rows); i++) { \
        for (int j = 0; j < (cols); j++) { \
            printf("%4d", MATRIX_GET(matrix, rows, cols, i, j)); \
        } \
        printf("\n"); \
    } \
} while(0)

#define MATRIX_ADD(result, a, b, rows, cols) do { \
    for (int i = 0; i < (rows); i++) { \
        for (int j = 0; j < (cols); j++) { \
            int sum = MATRIX_GET(a, rows, cols, i, j) + MATRIX_GET(b, rows, cols, i, j); \
            if (sum > MATRIX_ELEM_MAX) sum = MATRIX_ELEM_MAX; \
            if (sum < MATRIX_ELEM_MIN) sum = MATRIX_ELEM_MIN; \
            MATRIX_SET(result, rows, cols, i, j, sum); \
        } \
    } \
} while(0)

#define MATRIX_MULTIPLY(result, a, a_rows, a_cols, b, b_rows, b_cols) do { \
    for (int i = 0; i < (a_rows); i++) { \
        for (int j = 0; j < (b_cols); j++) { \
            int sum = 0; \
            for (int k = 0; k < (a_cols); k++) { \
                int term1 = MATRIX_GET(a, a_rows, a_cols, i, k); \
                int term2 = MATRIX_GET(b, b_rows, b_cols, k, j); \
                if (term1 != 0 && term2 != 0) { \
                    if (term1 > 0 && term2 > 0 && term1 > INT_MAX / term2) { \
                        sum = INT_MAX; \
                    } else if (term1 < 0 && term2 < 0 && term1 < INT_MAX / term2) { \
                        sum = INT_MAX; \
                    } else if (term1 > 0 && term2 < 0 && term1 > INT_MIN / term2) { \
                        sum = INT_MIN; \
                    } else if (term1 < 0 && term2 > 0 && term1 < INT_MIN / term2) { \
                        sum = INT_MIN; \
                    } else { \
                        int product = term1 * term2; \
                        if (sum > 0 && product > INT_MAX - sum) { \
                            sum = INT_MAX; \
                        } else if (sum < 0 && product < INT_MIN - sum) { \
                            sum = INT_MIN; \
                        } else { \
                            sum += product; \
                        } \
                    } \
                } \
            } \
            if (sum > MATRIX_ELEM_MAX) sum = MATRIX_ELEM_MAX; \
            if (sum < MATRIX_ELEM_MIN) sum = MATRIX_ELEM_MIN; \
            MATRIX_SET(result, a_rows, b_cols, i, j, sum); \
        } \
    } \
} while(0)

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min_val && value <= max_val) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min_val, max_val);
    }
}

void matrix_fill_random(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val = rand() % (MATRIX_ELEM_MAX - MATRIX_ELEM_MIN + 1) + MATRIX_ELEM_MIN;
            MATRIX_SET(matrix, rows, cols, i, j, val);
        }
    }
}

int main(void) {
    srand(42);
    
    printf("Matrix Operations Demo\n");