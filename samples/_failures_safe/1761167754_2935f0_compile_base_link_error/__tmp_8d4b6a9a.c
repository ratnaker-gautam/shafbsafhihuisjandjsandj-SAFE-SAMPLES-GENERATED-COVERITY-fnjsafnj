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
    double total_periods = periods * time;
    
    if (total_periods > 1000000) {
        fprintf(stderr, "Error: Too many compounding periods\n");
        return 1;
    }
    
    double amount = principal * pow(1 + effective_rate / periods, total_periods);
    
    if (!isfinite(amount)) {
        fprintf(stderr, "Error: Calculation resulted in invalid value\n");
        return 1;
    }
    
    double interest = amount - principal;
    
    printf("\nCompound Interest Calculation Results:\n");
    printf("Principal: $%.2f\n", principal);
    printf("Annual Rate: %.2f%%\n", rate);
    printf("Time: %.2f years\n", time);
    printf("Compounding: %d times per year\n", periods);
    printf("Final Amount: $%.2f\n", amount);
    printf("Total Interest: $%.2f\n", interest);
    
    return 0;
}