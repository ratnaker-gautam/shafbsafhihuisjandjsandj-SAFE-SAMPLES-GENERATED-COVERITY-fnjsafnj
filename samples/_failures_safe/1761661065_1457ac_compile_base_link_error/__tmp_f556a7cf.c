//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, result;
    int years, compounds;
    
    printf("Enter principal amount: ");
    if (scanf("%lf", &principal) != 1 || principal <= 0) {
        fprintf(stderr, "Error: Invalid principal amount\n");
        return 1;
    }
    
    printf("Enter annual interest rate (%%): ");
    if (scanf("%lf", &rate) != 1 || rate <= 0 || rate > 100) {
        fprintf(stderr, "Error: Invalid interest rate\n");
        return 1;
    }
    
    printf("Enter number of years: ");
    if (scanf("%d", &years) != 1 || years <= 0 || years > 100) {
        fprintf(stderr, "Error: Invalid number of years\n");
        return 1;
    }
    
    printf("Enter compounding frequency per year: ");
    if (scanf("%d", &compounds) != 1 || compounds <= 0 || compounds > 365) {
        fprintf(stderr, "Error: Invalid compounding frequency\n");
        return 1;
    }
    
    rate = rate / 100.0;
    
    double base = 1.0 + (rate / compounds);
    double exponent = compounds * years;
    
    if (exponent > 1000.0) {
        fprintf(stderr, "Error: Calculation would overflow\n");
        return 1;
    }
    
    result = principal * pow(base, exponent);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Calculation resulted in invalid value\n");
        return 1;
    }
    
    printf("Future value: $%.2lf\n", result);
    printf("Total interest earned: $%.2lf\n", result - principal);
    
    return 0;
}