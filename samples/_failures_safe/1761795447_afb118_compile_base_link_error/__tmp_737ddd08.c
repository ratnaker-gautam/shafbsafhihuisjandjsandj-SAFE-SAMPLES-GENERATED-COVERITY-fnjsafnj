//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int main() {
    double principal, rate, final_amount;
    int years;
    
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
    
    printf("Enter number of years: ");
    if (scanf("%d", &years) != 1) {
        fprintf(stderr, "Error: Invalid number of years\n");
        return 1;
    }
    
    if (years <= 0 || years > 100) {
        fprintf(stderr, "Error: Years must be between 1 and 100\n");
        return 1;
    }
    
    rate = rate / 100.0;
    
    if (rate == 0) {
        final_amount = principal;
    } else {
        final_amount = principal * pow(1.0 + rate, years);
        
        if (final_amount == HUGE_VAL || final_amount == -HUGE_VAL) {
            fprintf(stderr, "Error: Calculation overflow\n");
            return 1;
        }
    }
    
    printf("\nInvestment Summary:\n");
    printf("Principal: $%.2f\n", principal);
    printf("Annual Rate: %.2f%%\n", rate * 100);
    printf("Years: %d\n", years);
    printf("Final Amount: $%.2f\n", final_amount);
    
    double total_interest = final_amount - principal;
    printf("Total Interest: $%.2f\n", total_interest);
    
    double annual_growth = pow(final_amount / principal, 1.0 / years) - 1.0;
    printf("Effective Annual Growth: %.2f%%\n", annual_growth * 100);
    
    return 0;
}