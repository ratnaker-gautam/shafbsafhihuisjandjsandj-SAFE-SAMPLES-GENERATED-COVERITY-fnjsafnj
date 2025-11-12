//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 1000

struct Point {
    double x;
    double y;
};

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    struct Point points[MAX_POINTS];
    int num_points = 0;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (num_points < 2 || num_points > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return 1;
    }
    
    printf("Enter coordinates for %d points (x y):\n", num_points);
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return 1;
        }
    }
    
    double total_distance = 0.0;
    double min_distance = -1.0;
    double max_distance = 0.0;
    int min_i = 0, min_j = 0;
    int max_i = 0, max_j = 0;
    
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = calculate_distance(points[i], points[j]);
            total_distance += dist;
            
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
                min_i = i;
                min_j = j;
            }
            
            if (dist > max_distance) {
                max_distance = dist;
                max_i = i;
                max_j = j;
            }
        }
    }
    
    int num_pairs = num_points * (num_points - 1) / 2;
    double average_distance = total_distance / num_pairs;
    
    printf("\nDistance Analysis Results:\n");
    printf("Total pairs analyzed: %d\n", num_pairs);
    printf("Average distance: %.6f\n", average_distance);
    printf("Minimum distance: %.6f between points %d and %d\n", 
           min_distance, min_i, min_j);
    printf("Maximum distance: %.6f between points %d and %d\n", 
           max_distance, max_i, max_j);
    
    double variance_sum = 0.0;
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = calculate_distance(points[i], points[j]);
            double diff = dist - average_distance;
            variance_sum += diff * diff;
        }
    }
    
    double variance = variance_sum / num_pairs;
    double std_dev = sqrt(variance);
    
    printf("Standard deviation of distances: %.6f\n", std_dev);
    
    return 0;
}