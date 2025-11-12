//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: numeric_computation
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
    
    printf("Enter a positive integer (max %d): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &n) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (n <= 0) {
        fprintf(stderr, "Input must be positive\n");
        return 1;
    }
    
    if (n > MAX_INPUT) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    double result_for = compute_series_sum(n);
    double result_while = compute_series_sum_while(n);
    double result_do_while = compute_series_sum_do_while(n);
    
    printf("For loop result: %.15f\n", result_for);
    printf("While loop result: %.15f\n", result_while);
    printf("Do-while loop result: %.15f\n", result_do_while);
    
    double expected = 3.141592653589793 * 3.141592653589793 / 6.0;
    printf("Expected limit (pi^2/6): %.15f\n", expected);
    
    double for_error = fabs(result_for - expected);
    double while_error = fabs(result_while - expected);
    double do_while_error = fabs(result_do_while - expected);
    
    printf("For loop error: %.15f\n", for_error);
    printf("While loop error: %.15f\n", while_error);
    printf("Do-while loop error: %.15f\n", do_while_error);
    
    return 0;
}