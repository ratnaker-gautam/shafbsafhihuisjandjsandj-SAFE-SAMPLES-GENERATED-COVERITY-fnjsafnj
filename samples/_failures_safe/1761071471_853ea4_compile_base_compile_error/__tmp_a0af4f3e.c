//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_INPUT_LEN 256

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    int dot_count = 0;
    int digit_count = 0;
    int i = 0;
    if (str[i] == '-' || str[i] == '+') i++;
    for (; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (str[i] >= '0' && str[i] <= '9') {
            digit_count++;
        } else {
            return 0;
        }
    }
    return digit_count > 0;
}

double safe_strtod(const char *str, double min_val, double max_val) {
    char *endptr;
    double val = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') return NAN;
    if (val < min_val || val > max_val) return NAN;
    if (val == 0.0 && str[0] != '0') return NAN;
    return val;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double numbers[100];
    int count = 0;
    double sum = 0.0;
    double min_val = DBL_MAX;
    double max_val = -DBL_MAX;

    printf("Enter up to 100 numbers (one per line, empty line to finish):\n");

    while (count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
        if (input[0] == '\0') break;
        
        if (!is_valid_number(input)) {
            printf("Invalid number format. Please enter a valid number.\n");
            continue;
        }

        double num = safe_strtod(input, -1e100, 1e100);
        if (isnan(num)) {
            printf("Number out of range or invalid. Please enter a valid number.\n");
            continue;
        }

        numbers[count] = num;
        sum += num;
        if (num < min_val) min_val = num;
        if (num > max_val) max_val = num;
        count++;
    }

    if (count == 0) {
        printf("No numbers entered.\n");
        return 0;
    }

    double mean = sum / count;
    double variance = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        variance += diff * diff;
    }
    variance /= count;
    double stddev = sqrt(variance);

    printf("\nResults:\n");
    printf("Count: %d\n", count);
    printf("Sum: %.6f\n", sum);
    printf("Mean: %.6f\n", mean);
    printf("Min: %.6f\n", min_val);
    printf("Max: %.6f\n", max_val);
    printf("Standard deviation: %.6f\n", stddev);

    printf("\nSorted numbers:\n");
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (numbers[j] > numbers[j+1]) {
                double temp = numbers[j];
                numbers[j] = numbers[j+1];
                numbers[j+1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%.6f ", numbers[i]);
        if ((i + 1) % 5 == 0) printf("\n");
    }
    if (count % 5 != 0) printf("\n");

    return 0;
}