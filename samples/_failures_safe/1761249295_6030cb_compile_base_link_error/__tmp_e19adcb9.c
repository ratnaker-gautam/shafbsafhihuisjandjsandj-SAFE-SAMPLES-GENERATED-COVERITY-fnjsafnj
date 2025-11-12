//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, time;
    int periods;
    
    printf("Enter principal amount: ");
    if (scanf("%lf", &principal) != 1) {
        fprintf(stderr, "Error: Invalid principal amount\n");
        return 1;
    }
    
    if (principal <= 0) {
        fprintf(stderr, "Error: Principal must be positive\n");
        return 1;
    }
    
    printf("Enter annual interest rate (%%): ");
    if (scanf("%lf", &rate) != 1) {
        fprintf(stderr, "Error: Invalid interest rate\n");
        return 1;
    }
    
    if (rate < 0 || rate > 100) {
        fprintf(stderr, "Error: Interest rate must be between 0 and 100\n");
        return 1;
    }
    
    printf("Enter time in years: ");
    if (scanf("%lf", &time) != 1) {
        fprintf(stderr, "Error: Invalid time period\n");
        return 1;
    }
    
    if (time <= 0) {
        fprintf(stderr, "Error: Time must be positive\n");
        return 1;
    }
    
    printf("Enter compounding periods per year: ");
    if (scanf("%d", &periods) != 1) {
        fprintf(stderr, "Error: Invalid compounding periods\n");
        return 1;
    }
    
    if (periods <= 0) {
        fprintf(stderr, "Error: Compounding periods must be positive\n");
        return 1;
    }
    
    double effective_rate = rate / 100.0;
    double compound_factor = pow(1.0 + effective_rate / periods, periods * time);
    
    if (!isfinite(compound_factor)) {
        fprintf(stderr, "Error: Calculation resulted in overflow\n");
        return 1;
    }
    
    double future_value = principal * compound_factor;
    
    if (!isfinite(future_value)) {
        fprintf(stderr, "Error: Calculation resulted in overflow\n");
        return 1;
    }
    
    printf("\nInvestment Summary:\n");
    printf("Principal: $%.2f\n", principal);
    printf("Annual Rate: %.2f%%\n", rate);
    printf("Time: %.2f years\n", time);
    printf("Compounding: %d times per year\n", periods);
    printf("Future Value: $%.2f\n", future_value);
    printf("Total Interest: $%.2f\n", future_value - principal);
    
    return 0;
}