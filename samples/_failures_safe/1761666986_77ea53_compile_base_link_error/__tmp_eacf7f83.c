//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, time;
    int n;
    
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
    
    printf("Enter number of compounding periods per year: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid compounding periods\n");
        return 1;
    }
    
    if (n <= 0) {
        fprintf(stderr, "Error: Compounding periods must be positive\n");
        return 1;
    }
    
    double r = rate / 100.0;
    double amount = principal * pow(1 + r / n, n * time);
    double interest = amount - principal;
    
    printf("\nCompound Interest Calculation Results:\n");
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate);
    printf("Time: %.2lf years\n", time);
    printf("Compounding: %d times per year\n", n);
    printf("Final Amount: $%.2lf\n", amount);
    printf("Total Interest: $%.2lf\n", interest);
    
    printf("\nYear-by-Year Breakdown:\n");
    double current = principal;
    for (int year = 1; year <= (int)time; year++) {
        double yearly_interest = 0;
        for (int period = 0; period < n; period++) {
            double period_interest = current * (r / n);
            yearly_interest += period_interest;
            current += period_interest;
        }
        printf("Year %d: $%.2lf (Interest: $%.2lf)\n", year, current, yearly_interest);
    }
    
    if (time > (int)time) {
        double remaining_time = time - (int)time;
        double partial_interest = current * (r / n) * (remaining_time * n);
        current += partial_interest;
        printf("Partial period: $%.2lf (Interest: $%.2lf)\n", current, partial_interest);
    }
    
    return 0;
}