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
    
    if (n <= 0 || n > 365) {
        fprintf(stderr, "Error: Compounding periods must be between 1 and 365\n");
        return 1;
    }
    
    double r = rate / 100.0;
    double amount = principal * pow(1 + r/n, n * time);
    double interest = amount - principal;
    
    printf("\n--- Compound Interest Calculation ---\n");
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate);
    printf("Time: %.2lf years\n", time);
    printf("Compounding: %d times per year\n", n);
    printf("Final Amount: $%.2lf\n", amount);
    printf("Total Interest: $%.2lf\n", interest);
    
    printf("\nYear-by-Year Breakdown:\n");
    printf("Year    Amount\n");
    printf("----    ------\n");
    
    for (int year = 1; year <= (int)time; year++) {
        double year_amount = principal * pow(1 + r/n, n * year);
        printf("%-4d    $%.2lf\n", year, year_amount);
    }
    
    if (time > (int)time) {
        double partial_amount = principal * pow(1 + r/n, n * time);
        printf("%.1f     $%.2lf\n", time, partial_amount);
    }
    
    double doubling_time = log(2) / log(1 + r/n) / n;
    printf("\nTime to double investment: %.2lf years\n", doubling_time);
    
    return 0;
}