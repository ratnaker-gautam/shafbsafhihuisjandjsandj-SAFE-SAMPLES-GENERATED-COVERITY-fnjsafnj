//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_INPUTS 100

int validate_input_count(int count) {
    if (count <= 0 || count > MAX_INPUTS) {
        return 0;
    }
    return 1;
}

int read_numbers(double numbers[], int count) {
    printf("Enter %d numbers:\n", count);
    for (int i = 0; i < count; i++) {
        if (scanf("%lf", &numbers[i]) != 1) {
            return 0;
        }
    }
    return 1;
}

double compute_mean(const double numbers[], int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    return sum / count;
}

double compute_stddev(const double numbers[], int count, double mean) {
    double sum_sq = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / count);
}

void sort_numbers(double numbers[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                double temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
}

double compute_median(double numbers[], int count) {
    sort_numbers(numbers, count);
    if (count % 2 == 0) {
        return (numbers[count / 2 - 1] + numbers[count / 2]) / 2.0;
    } else {
        return numbers[count / 2];
    }
}

void print_results(double mean, double stddev, double median) {
    printf("Statistical Analysis Results:\n");
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", stddev);
    printf("Median: %.6f\n", median);
}

int main(void) {
    int count;
    double numbers[MAX_INPUTS];
    
    printf("Enter number of values to analyze (1-%d): ", MAX_INPUTS);
    if (scanf("%d", &count) != 1) {
        fprintf(stderr, "Error: Invalid input for count\n");
        return 1;
    }
    
    if (!validate_input_count(count)) {
        fprintf(stderr, "Error: Count must be between 1 and %d\n", MAX_INPUTS);
        return 1;
    }
    
    if (!read_numbers(numbers, count)) {
        fprintf(stderr, "Error: Invalid number input\n");
        return 1;
    }
    
    double mean = compute_mean(numbers, count);
    double stddev = compute_stddev(numbers, count, mean);
    double median = compute_median(numbers, count);
    
    print_results(mean, stddev, median);
    
    return 0;
}