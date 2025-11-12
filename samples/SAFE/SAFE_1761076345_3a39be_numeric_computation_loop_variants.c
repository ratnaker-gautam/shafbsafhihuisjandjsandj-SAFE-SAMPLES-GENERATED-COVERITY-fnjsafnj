//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        if (i == 0) continue;
        sum += 1.0 / ((double)i * (double)i);
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        if (i == 0) {
            i++;
            continue;
        }
        sum += 1.0 / ((double)i * (double)i);
        i++;
    }
    return sum;
}

double compute_series_sum_do_while(int n) {
    double sum = 0.0;
    int i = 1;
    if (n > 0) {
        do {
            if (i == 0) {
                i++;
                continue;
            }
            sum += 1.0 / ((double)i * (double)i);
            i++;
        } while (i <= n);
    }
    return sum;
}

int main(void) {
    int n;
    char input_buffer[32];
    
    printf("Enter a positive integer (1-%d): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n <= 0 || n > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("Series sum using for loop: %.10f\n", result_for);
    printf("Series sum using while loop: %.10f\n", result_while);
    printf("Series sum using do-while loop: %.10f\n", result_do_while);
    
    double expected_limit = 3.14159265358979323846 * 3.14159265358979323846 / 6.0;
    printf("Theoretical limit (π²/6): %.10f\n", expected_limit);
    
    return 0;
}