//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, result;
    int years, compounds;
    
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
    
    printf("Enter number of years: ");
    if (scanf("%d", &years) != 1 || years <= 0 || years > 100) {
        printf("Invalid number of years\n");
        return 1;
    }
    
    printf("Enter compounding frequency per year: ");
    if (scanf("%d", &compounds) != 1 || compounds <= 0 || compounds > 365) {
        printf("Invalid compounding frequency\n");
        return 1;
    }
    
    double effective_rate = rate / 100.0;
    double base = 1.0 + effective_rate / compounds;
    double exponent = compounds * years;
    
    if (exponent > 1000.0) {
        printf("Computation would exceed safe limits\n");
        return 1;
    }
    
    result = principal * pow(base, exponent);
    
    if (!isfinite(result)) {
        printf("Computation resulted in invalid value\n");
        return 1;
    }
    
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate);
    printf("Years: %d\n", years);
    printf("Compounding: %d times per year\n", compounds);
    printf("Future Value: $%.2lf\n", result);
    
    return 0;
}