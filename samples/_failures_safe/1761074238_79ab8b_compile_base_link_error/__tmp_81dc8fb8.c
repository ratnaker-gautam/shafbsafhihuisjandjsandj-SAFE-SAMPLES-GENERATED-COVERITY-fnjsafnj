//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, time;
    int periods;
    
    printf("Enter principal amount: ");
    if (scanf("%lf", &principal) != 1 || principal <= 0) {
        printf("Invalid principal amount\n");
        return 1;
    }
    
    printf("Enter annual interest rate (%%): ");
    if (scanf("%lf", &rate) != 1 || rate <= 0) {
        printf("Invalid interest rate\n");
        return 1;
    }
    
    printf("Enter time in years: ");
    if (scanf("%lf", &time) != 1 || time <= 0) {
        printf("Invalid time period\n");
        return 1;
    }
    
    printf("Enter compounding periods per year: ");
    if (scanf("%d", &periods) != 1 || periods <= 0) {
        printf("Invalid compounding periods\n");
        return 1;
    }
    
    rate = rate / 100.0;
    double effective_rate = rate / periods;
    double total_periods = periods * time;
    
    if (total_periods > 1000000) {
        printf("Calculation period too large\n");
        return 1;
    }
    
    double amount = principal * pow(1.0 + effective_rate, total_periods);
    
    if (!isfinite(amount)) {
        printf("Calculation resulted in overflow\n");
        return 1;
    }
    
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate * 100);
    printf("Time: %.2lf years\n", time);
    printf("Compounding Periods: %d per year\n", periods);
    printf("Future Value: $%.2lf\n", amount);
    printf("Total Interest: $%.2lf\n", amount - principal);
    
    return 0;
}