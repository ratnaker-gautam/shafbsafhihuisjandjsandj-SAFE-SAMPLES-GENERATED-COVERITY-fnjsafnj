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
    int num_points;
    struct Point points[MAX_POINTS];
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_points < 2 || num_points > MAX_POINTS) {
        printf("Number of points must be between 2 and %d\n", MAX_POINTS);
        return 1;
    }
    
    printf("Enter coordinates for %d points (x y):\n", num_points);
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            printf("Invalid coordinate input\n");
            return 1;
        }
    }
    
    double total_distance = 0.0;
    double min_distance = -1.0;
    double max_distance = 0.0;
    
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = calculate_distance(points[i], points[j]);
            total_distance += dist;
            
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
            }
            
            if (dist > max_distance) {
                max_distance = dist;
            }
        }
    }
    
    int total_pairs = num_points * (num_points - 1) / 2;
    double average_distance = total_distance / total_pairs;
    
    printf("\nDistance Statistics:\n");
    printf("Total pairs: %d\n", total_pairs);
    printf("Minimum distance: %.6f\n", min_distance);
    printf("Maximum distance: %.6f\n", max_distance);
    printf("Average distance: %.6f\n", average_distance);
    
    double sum_squared_diff = 0.0;
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = calculate_distance(points[i], points[j]);
            double diff = dist - average_distance;
            sum_squared_diff += diff * diff;
        }
    }
    
    double std_deviation = sqrt(sum_squared_diff / total_pairs);
    printf("Standard deviation: %.6f\n", std_deviation);
    
    return 0;
}