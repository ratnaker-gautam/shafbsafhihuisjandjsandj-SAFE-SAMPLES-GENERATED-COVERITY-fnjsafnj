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
        double segment_distance = calculate_distance(points[i], points[i + 1]);
        total_distance += segment_distance;
        printf("Distance from point %d to point %d: %.6f\n", i + 1, i + 2, segment_distance);
    }
    
    printf("Total path distance: %.6f\n", total_distance);
    
    double min_distance = calculate_distance(points[0], points[1]);
    double max_distance = min_distance;
    
    for (int i = 0; i < num_points; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (dist < min_distance) {
                min_distance = dist;
            }
            if (dist > max_distance) {
                max_distance = dist;
            }
        }
    }
    
    printf("Minimum distance between any two points: %.6f\n", min_distance);
    printf("Maximum distance between any two points: %.6f\n", max_distance);
    
    double center_x = 0.0, center_y = 0.0;
    for (int i = 0; i < num_points; i++) {
        center_x += points[i].x;
        center_y += points[i].y;
    }
    center_x /= num_points;
    center_y /= num_points;
    
    printf("Geometric center: (%.6f, %.6f)\n", center_x, center_y);
    
    double max_radius = 0.0;
    for (int i = 0; i < num_points; i++) {
        double radius = calculate_distance(points[i], (struct Point){center_x, center_y});
        if (radius > max_radius) {
            max_radius = radius;
        }
    }
    
    printf("Bounding circle radius: %.6f\n", max_radius);
    
    return 0;
}