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
    int num_points;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (num_points < 2 || num_points > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return 1;
    }
    
    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < num_points; i++) {
        printf("Point %d: ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return 1;
        }
    }
    
    double total_distance = 0.0;
    int valid_segments = 0;
    
    printf("\nCalculating distances between consecutive points:\n");
    for (int i = 0; i < num_points - 1; i++) {
        double distance = calculate_distance(points[i], points[i + 1]);
        if (!isnan(distance) && !isinf(distance)) {
            printf("Distance from point %d to point %d: %.6f\n", i + 1, i + 2, distance);
            total_distance += distance;
            valid_segments++;
        } else {
            printf("Warning: Invalid distance between points %d and %d\n", i + 1, i + 2);
        }
    }
    
    if (valid_segments > 0) {
        printf("\nTotal distance: %.6f\n", total_distance);
        printf("Average distance per segment: %.6f\n", total_distance / valid_segments);
    } else {
        printf("\nNo valid distance segments found\n");
    }
    
    double min_x = points[0].x;
    double max_x = points[0].x;
    double min_y = points[0].y;
    double max_y = points[0].y;
    
    for (int i = 1; i < num_points; i++) {
        if (points[i].x < min_x) min_x = points[i].x;
        if (points[i].x > max_x) max_x = points[i].x;
        if (points[i].y < min_y) min_y = points[i].y;
        if (points[i].y > max_y) max_y = points[i].y;
    }
    
    printf("\nBounding box:\n");
    printf("X range: [%.6f, %.6f]\n", min_x, max_x);
    printf("Y range: [%.6f, %.6f]\n", min_y, max_y);
    printf("Width: %.6f, Height: %.6f\n", max_x - min_x, max_y - min_y);
    
    return 0;
}