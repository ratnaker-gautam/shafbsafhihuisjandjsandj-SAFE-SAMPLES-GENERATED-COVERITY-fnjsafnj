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
    
    printf("Enter annual interest rate (as percentage): ");
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
    
    rate = rate / 100.0;
    
    double temp = 1.0 + (rate / compounds);
    if (temp <= 0) {
        printf("Calculation error\n");
        return 1;
    }
    
    double exponent = (double)years * compounds;
    if (exponent > 10000) {
        printf("Exponent too large for calculation\n");
        return 1;
    }
    
    double growth_factor = pow(temp, exponent);
    if (isinf(growth_factor) || isnan(growth_factor)) {
        printf("Calculation overflow\n");
        return 1;
    }
    
    result = principal * growth_factor;
    
    if (isinf(result) || isnan(result)) {
        printf("Result overflow\n");
        return 1;
    }
    
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate * 100);
    printf("Years: %d\n", years);
    printf("Compounds per year: %d\n", compounds);
    printf("Future Value: $%.2lf\n", result);
    
    return 0;
}