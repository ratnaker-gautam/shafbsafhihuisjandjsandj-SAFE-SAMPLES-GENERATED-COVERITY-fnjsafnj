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
    double total_distance = 0.0;
    
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
    
    for (int i = 0; i < num_points - 1; i++) {
        double dist = calculate_distance(points[i], points[i + 1]);
        total_distance += dist;
    }
    
    printf("Total distance along the path: %.6f\n", total_distance);
    
    double min_dist = -1.0;
    double max_dist = -1.0;
    for (int i = 0; i < num_points - 1; i++) {
        double dist = calculate_distance(points[i], points[i + 1]);
        if (min_dist < 0 || dist < min_dist) {
            min_dist = dist;
        }
        if (max_dist < 0 || dist > max_dist) {
            max_dist = dist;
        }
    }
    
    printf("Minimum segment distance: %.6f\n", min_dist);
    printf("Maximum segment distance: %.6f\n", max_dist);
    
    return 0;
}