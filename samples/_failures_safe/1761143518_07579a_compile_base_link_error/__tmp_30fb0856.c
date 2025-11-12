//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 100

int main() {
    double data[MAX_SIZE];
    double *ptr = data;
    double sum = 0.0;
    double mean = 0.0;
    double variance = 0.0;
    int count = 0;
    char buffer[256];
    
    printf("Enter up to %d numbers (empty line to finish):\n", MAX_SIZE);
    
    while (count < MAX_SIZE) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (buffer[0] == '\n') {
            break;
        }
        
        char *endptr;
        double value = strtod(buffer, &endptr);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            printf("Number out of range. Please enter a valid number.\n");
            continue;
        }
        
        *(ptr + count) = value;
        sum += value;
        count++;
    }
    
    if (count == 0) {
        printf("No valid numbers entered.\n");
        return 1;
    }
    
    mean = sum / count;
    
    double *current = data;
    for (int i = 0; i < count; i++) {
        double diff = *(current + i) - mean;
        variance += diff * diff;
    }
    variance /= count;
    
    double stddev = sqrt(variance);
    
    printf("Statistics for %d numbers:\n", count);
    printf("Mean: %.6f\n", mean);
    printf("Variance: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", stddev);
    
    printf("Values sorted by distance from mean:\n");
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            double dist1 = fabs(*(data + j) - mean);
            double dist2 = fabs(*(data + j + 1) - mean);
            if (dist1 > dist2) {
                double temp = *(data + j);
                *(data + j) = *(data + j + 1);
                *(data + j + 1) = temp;
            }
        }
    }
    
    for (int i = 0; i < count; i++) {
        printf("%.6f (distance: %.6f)\n", *(data + i), fabs(*(data + i) - mean));
    }
    
    return 0;
}