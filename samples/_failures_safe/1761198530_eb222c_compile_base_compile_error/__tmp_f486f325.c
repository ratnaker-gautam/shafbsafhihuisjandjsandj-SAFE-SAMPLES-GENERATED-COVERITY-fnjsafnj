//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

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
    
    double decimal_rate = rate / 100.0;
    
    if (decimal_rate > 0 && years > log(DBL_MAX / principal) / log(1.0 + decimal_rate)) {
        fprintf(stderr, "Error: Calculation would overflow\n");
        return 1;
    }
    
    final_amount = principal * pow(1.0 + decimal_rate, years);
    
    if (!isfinite(final_amount)) {
        fprintf(stderr, "Error: Calculation resulted in invalid value\n");
        return 1;
    }
    
    printf("\nCompound Interest Calculation Results:\n");
    printf("Principal: $%.2lf\n", principal);
    printf("Annual Rate: %.2lf%%\n", rate);
    printf("Years: %d\n", years);
    printf("Final Amount: $%.2lf\n", final_amount);
    
    double total_interest = final_amount - principal;
    printf("Total Interest Earned: $%.2lf\n", total_interest);
    
    double annual_growth = (pow(final_amount / principal, 1.0 / years) - 1.0) * 100.0;
    printf("Effective Annual Growth Rate: %.2lf%%\n", annual_growth);
    
    printf("\nYear-by-Year Breakdown:\n");
    printf("Year    Balance      Interest\n");
    printf("----    -------      --------\n");
    
    double current_balance = principal;
    for (int year = 1; year <= years; year++) {
        double yearly_interest = current_balance * decimal_rate;
        if (!isfinite(yearly_interest)) {
            fprintf(stderr, "Error: Yearly calculation overflow\n");
            return 1;
        }
        current_balance += yearly_interest;
        
        if (!isfinite(current_balance)) {
            fprintf(stderr, "Error: Balance calculation overflow\n");
            return 1;
        }
        
        printf("%4d    $%9.2lf  $%9.2lf\n", year, current_balance, yearly_interest);
    }
    
    return 0;
}