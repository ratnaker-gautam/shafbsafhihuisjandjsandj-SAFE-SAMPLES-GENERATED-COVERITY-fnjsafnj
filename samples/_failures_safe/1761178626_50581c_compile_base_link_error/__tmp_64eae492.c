//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_compound_interest(double principal, double rate, int years, int compounds_per_year) {
    if (principal <= 0.0 || rate < 0.0 || years < 0 || compounds_per_year <= 0) {
        return -1.0;
    }
    
    if (rate > 1000.0 || years > 1000 || compounds_per_year > 365) {
        return -1.0;
    }
    
    double amount = principal * pow(1.0 + rate / (100.0 * compounds_per_year), compounds_per_year * years);
    
    if (amount == HUGE_VAL || amount == -HUGE_VAL || amount != amount) {
        return -1.0;
    }
    
    return amount;
}

int read_double(const char* prompt, double* value) {
    char buffer[MAX_INPUT_LEN];
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char* endptr;
    *value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    
    return 1;
}

int read_int(const char* prompt, int* value) {
    char buffer[MAX_INPUT_LEN];
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    char* endptr;
    long temp = strtol(buffer, &endptr, 10);
    
    if (endptr == buffer || *endptr != '\n' || temp < INT_MIN || temp > INT_MAX) {
        return 0;
    }
    
    *value = (int)temp;
    return 1;
}

int main(void) {
    double principal, rate;
    int years, compounds;
    
    printf("Compound Interest Calculator\n");
    printf("============================\n\n");
    
    if (!read_double("Enter principal amount: ", &principal)) {
        printf("Error: Invalid principal amount\n");
        return 1;
    }
    
    if (principal <= 0.0) {
        printf("Error: Principal must be positive\n");
        return 1;
    }
    
    if (!read_double("Enter annual interest rate (%%): ", &rate)) {
        printf("Error: Invalid interest rate\n");
        return 1;
    }
    
    if (rate < 0.0 || rate > 1000.0) {
        printf("Error: Interest rate must be between 0 and 1000%%\n");
        return 1;
    }
    
    if (!read_int("Enter number of years: ", &years)) {
        printf("Error: Invalid number of years\n");
        return 1;
    }
    
    if (years <= 0 || years > 1000) {
        printf("Error: Years must be between 1 and 1000\n");
        return 1;
    }
    
    if (!read_int("Enter compounding frequency per year: ", &compounds)) {
        printf("Error: Invalid compounding frequency\n");
        return 1;
    }
    
    if (compounds <= 0 || compounds > 365) {
        printf("Error: Compounding frequency must be between 1 and 365\n");
        return 1;
    }
    
    double result = calculate_compound_interest(principal, rate, years, compounds);
    
    if (result < 0.0) {
        printf("Error: Calculation failed - invalid input or overflow\n");
        return 1;
    }
    
    printf("\nResults:\n");
    printf("Principal: $%.2f\n", principal);
    printf("Annual Rate: %.2f%%\n", rate);
    printf("Years: %d\n", years);
    printf("Compounding: %d times per year\n", compounds);
    printf("Final Amount: $%.2f\n", result);
    printf("Total Interest: $%.2f\n", result - principal);
    
    return 0;
}