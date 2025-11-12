//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, time;
    int n;
    
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
    
    printf("Enter number of compounding periods per year: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid compounding periods\n");
        return 1;
    }
    
    rate = rate / 100.0;
    
    double amount = principal * pow(1 + rate / n, n * time);
    double interest = amount - principal;
    
    printf("\nCompound Interest Calculation Results:\n");
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate * 100);
    printf("Time: %.2lf years\n", time);
    printf("Compounding: %d times per year\n", n);
    printf("Final Amount: $%.2lf\n", amount);
    printf("Total Interest: $%.2lf\n", interest);
    
    printf("\nYear-by-Year Breakdown:\n");
    double current = principal;
    for (int year = 1; year <= (int)time; year++) {
        double yearly_amount = principal * pow(1 + rate / n, n * year);
        double yearly_interest = yearly_amount - current;
        current = yearly_amount;
        printf("Year %d: $%.2lf (Interest: $%.2lf)\n", year, yearly_amount, yearly_interest);
    }
    
    if (time > (int)time) {
        double partial_amount = principal * pow(1 + rate / n, n * time);
        double partial_interest = partial_amount - current;
        printf("Partial period: $%.2lf (Interest: $%.2lf)\n", partial_amount, partial_interest);
    }
    
    return 0;
}