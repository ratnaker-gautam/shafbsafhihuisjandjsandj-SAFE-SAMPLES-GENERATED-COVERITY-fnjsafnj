//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

double compute_series_sum(int n) {
    double sum = 0.0;
    int i;
    
    if (n <= 0) {
        return 0.0;
    }
    
    for (i = 1; i <= n; i++) {
        sum += 1.0 / (i * i);
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    
    if (n <= 0) {
        return 0.0;
    }
    
    while (i <= n) {
        sum += 1.0 / (i * i);
        i++;
    }
    return sum;
}

double compute_series_sum_do_while(int n) {
    double sum = 0.0;
    int i = 1;
    
    if (n <= 0) {
        return 0.0;
    }
    
    do {
        sum += 1.0 / (i * i);
        i++;
    } while (i <= n);
    
    return sum;
}

int main(void) {
    int n;
    char input_buffer[32];
    char *endptr;
    
    printf("Enter number of terms for series computation (1-%d): ", MAX_ITERATIONS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    n = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input: not a valid integer\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_ITERATIONS) {
        fprintf(stderr, "Invalid input: must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }
    
    printf("Computing sum of 1/k^2 from k=1 to %d using different loop variants:\n", n);
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("For loop result: %.15f\n", result_for);
    printf("While loop result: %.15f\n", result_while);
    printf("Do-while loop result: %.15f\n", result_do_while);
    
    double diff1 = fabs(result_for - result_while);
    double diff2 = fabs(result_for - result_do_while);
    
    if (diff1 < 1e-15 && diff2 < 1e-15) {
        printf("All loop variants produced identical results\n");
    } else {
        printf("Warning: Loop variants produced different results\n");
    }
    
    double pi_approximation = sqrt(6.0 * result_for);
    printf("Pi approximation: %.15f\n", pi_approximation);
    
    return 0;
}