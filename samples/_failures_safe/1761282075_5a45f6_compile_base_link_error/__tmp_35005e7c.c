//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    double principal, rate, result;
    int years, compounds;
    
    printf("Enter principal amount: ");
    if (scanf("%lf", &principal) != 1 || principal <= 0) {
        fprintf(stderr, "Invalid principal amount\n");
        return 1;
    }
    
    printf("Enter annual interest rate (as percentage): ");
    if (scanf("%lf", &rate) != 1 || rate <= 0 || rate > 100) {
        fprintf(stderr, "Invalid interest rate\n");
        return 1;
    }
    
    printf("Enter number of years: ");
    if (scanf("%d", &years) != 1 || years <= 0 || years > 100) {
        fprintf(stderr, "Invalid number of years\n");
        return 1;
    }
    
    printf("Enter compounding frequency per year: ");
    if (scanf("%d", &compounds) != 1 || compounds <= 0 || compounds > 365) {
        fprintf(stderr, "Invalid compounding frequency\n");
        return 1;
    }
    
    double decimal_rate = rate / 100.0;
    
    if (compounds == 1) {
        result = principal * pow(1.0 + decimal_rate, years);
    } else {
        double temp = 1.0 + (decimal_rate / compounds);
        double exponent = compounds * years;
        
        if (exponent > 1000.0) {
            fprintf(stderr, "Exponent too large for computation\n");
            return 1;
        }
        
        result = principal * pow(temp, exponent);
    }
    
    if (!isfinite(result)) {
        fprintf(stderr, "Numerical overflow in calculation\n");
        return 1;
    }
    
    printf("Principal: $%.2lf\n", principal);
    printf("Annual rate: %.2lf%%\n", rate);
    printf("Years: %d\n", years);
    printf("Compounding: %d times per year\n", compounds);
    printf("Future value: $%.2lf\n", result);
    
    return 0;
}