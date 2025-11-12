//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define MAX_INPUT 1000000

double compute_series_sum(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / (i * i);
    }
    return sum;
}

double compute_series_sum_while(int n) {
    double sum = 0.0;
    int i = 1;
    while (i <= n) {
        sum += 1.0 / (i * i);
        i++;
    }
    return sum;
}

double compute_series_sum_dowhile(int n) {
    double sum = 0.0;
    int i = 1;
    if (n < 1) return 0.0;
    do {
        sum += 1.0 / (i * i);
        i++;
    } while (i <= n);
    return sum;
}

int main() {
    char input_buffer[32];
    int n;
    
    printf("Enter number of terms to compute series sum (1-%d): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n < 1 || n > MAX_INPUT) {
        fprintf(stderr, "Input out of range\n");
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_dowhile = compute_series_sum_dowhile(n);
    
    printf("For loop result: %.10f\n", result_for);
    printf("While loop result: %.10f\n", result_while);
    printf("Do-while loop result: %.10f\n", result_dowhile);
    
    double diff1 = fabs(result_for - result_while);
    double diff2 = fabs(result_for - result_dowhile);
    
    if (diff1 < 1e-12 && diff2 < 1e-12) {
        printf("All methods agree within tolerance\n");
    } else {
        printf("Warning: Methods show significant differences\n");
    }
    
    return 0;
}