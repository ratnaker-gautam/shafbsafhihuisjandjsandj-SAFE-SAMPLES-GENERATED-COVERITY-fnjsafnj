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
    if (scanf("%lf", &rate) != 1 || rate <= 0 || rate > 100) {
        printf("Invalid interest rate\n");
        return 1;
    }
    
    printf("Enter time in years: ");
    if (scanf("%lf", &time) != 1 || time <= 0) {
        printf("Invalid time period\n");
        return 1;
    }
    
    printf("Enter number of compounding periods per year: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 365) {
        printf("Invalid compounding frequency\n");
        return 1;
    }
    
    rate = rate / 100.0;
    
    double amount = principal * pow(1 + rate / n, n * time);
    
    if (amount == HUGE_VAL || amount != amount) {
        printf("Calculation error: result overflow\n");
        return 1;
    }
    
    double interest = amount - principal;
    
    printf("\nInvestment Summary:\n");
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate * 100);
    printf("Time: %.2lf years\n", time);
    printf("Compounding: %d times per year\n", n);
    printf("Final Amount: $%.2lf\n", amount);
    printf("Total Interest: $%.2lf\n", interest);
    
    return 0;
}