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
    
    rate = rate / 100.0;
    
    double temp = 1.0 + (rate / compounds);
    if (temp <= 0) {
        printf("Calculation error\n");
        return 1;
    }
    
    double exponent = (double)years * compounds;
    if (exponent > 10000.0) {
        printf("Exponent too large for calculation\n");
        return 1;
    }
    
    double growth_factor = pow(temp, exponent);
    if (growth_factor == HUGE_VAL || growth_factor == -HUGE_VAL) {
        printf("Calculation overflow\n");
        return 1;
    }
    
    result = principal * growth_factor;
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        printf("Result overflow\n");
        return 1;
    }
    
    printf("Future value: $%.2lf\n", result);
    printf("Total interest earned: $%.2lf\n", result - principal);
    
    return 0;
}