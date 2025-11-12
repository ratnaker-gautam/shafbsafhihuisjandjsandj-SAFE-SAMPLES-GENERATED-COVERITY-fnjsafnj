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
        printf("Point %d: ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return 1;
        }
    }
    
    printf("\nCalculating total path distance...\n");
    for (int i = 0; i < num_points - 1; i++) {
        double segment_distance = calculate_distance(points[i], points[i + 1]);
        total_distance += segment_distance;
        printf("Segment %d-%d: %.6f\n", i + 1, i + 2, segment_distance);
    }
    
    printf("\nTotal path distance: %.6f\n", total_distance);
    
    double mean_distance = total_distance / (num_points - 1);
    printf("Mean segment distance: %.6f\n", mean_distance);
    
    double variance = 0.0;
    for (int i = 0; i < num_points - 1; i++) {
        double segment_distance = calculate_distance(points[i], points[i + 1]);
        double diff = segment_distance - mean_distance;
        variance += diff * diff;
    }
    variance /= (num_points - 1);
    
    printf("Variance of segment distances: %.6f\n", variance);
    printf("Standard deviation: %.6f\n", sqrt(variance));
    
    return 0;
}