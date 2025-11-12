//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define MAX_INPUT 100

int main() {
    char input[MAX_INPUT + 1];
    int n;
    
    printf("Enter a positive integer (1-100): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    long temp = strtol(input, &endptr, 10);
    if (endptr == input || (*endptr != '\n' && *endptr != '\0')) {
        fprintf(stderr, "Invalid input: not a valid integer\n");
        return 1;
    }
    
    if (temp < 1 || temp > 100) {
        fprintf(stderr, "Input out of range (1-100)\n");
        return 1;
    }
    n = (int)temp;
    
    printf("Computing sum of squares using different loop variants:\n\n");
    
    long sum_for = 0;
    for (int i = 1; i <= n; i++) {
        long square = (long)i * (long)i;
        if (i > 0 && sum_for > LONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_for += square;
    }
    printf("For loop sum: %ld\n", sum_for);
    
    long sum_while = 0;
    int j = 1;
    while (j <= n) {
        long square = (long)j * (long)j;
        if (j > 0 && sum_while > LONG_MAX - square) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        sum_while += square;
        j++;
    }
    printf("While loop sum: %ld\n", sum_while);
    
    long sum_do_while = 0;
    int k = 1;
    if (n >= 1) {
        do {
            long square = (long)k * (long)k;
            if (k > 0 && sum_do_while > LONG_MAX - square) {
                fprintf(stderr, "Integer overflow detected\n");
                return 1;
            }
            sum_do_while += square;
            k++;
        } while (k <= n);
    }
    printf("Do-while loop sum: %ld\n", sum_do_while);
    
    double closed_form = (double)n * (n + 1) * (2 * n + 1) / 6.0;
    printf("Closed form (n(n+1)(2n+1)/6): %.0f\n", closed_form);
    
    double error = fabs((double)sum_for - closed_form);
    printf("Numerical error: %e\n", error);
    
    return 0;
}