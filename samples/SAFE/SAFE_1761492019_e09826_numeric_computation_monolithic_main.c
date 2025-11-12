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
    
    printf("Enter coordinates for %d points:\n", num_points);
    for (int i = 0; i < num_points; i++) {
        printf("Point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return 1;
        }
    }
    
    for (int i = 0; i < num_points - 1; i++) {
        double segment_distance = calculate_distance(points[i], points[i + 1]);
        total_distance += segment_distance;
        printf("Distance from point %d to point %d: %.6f\n", 
               i + 1, i + 2, segment_distance);
    }
    
    printf("Total path distance: %.6f\n", total_distance);
    
    double min_distance = -1.0;
    int min_i = -1, min_j = -1;
    
    for (int i = 0; i < num_points; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
                min_i = i;
                min_j = j;
            }
        }
    }
    
    if (min_distance >= 0) {
        printf("Closest points: %d (%.3f,%.3f) and %d (%.3f,%.3f)\n",
               min_i + 1, points[min_i].x, points[min_i].y,
               min_j + 1, points[min_j].x, points[min_j].y);
        printf("Minimum distance: %.6f\n", min_distance);
    }
    
    double center_x = 0.0, center_y = 0.0;
    for (int i = 0; i < num_points; i++) {
        center_x += points[i].x;
        center_y += points[i].y;
    }
    center_x /= num_points;
    center_y /= num_points;
    
    printf("Centroid of all points: (%.6f, %.6f)\n", center_x, center_y);
    
    double max_distance_from_center = 0.0;
    for (int i = 0; i < num_points; i++) {
        struct Point center = {center_x, center_y};
        double dist = calculate_distance(points[i], center);
        if (dist > max_distance_from_center) {
            max_distance_from_center = dist;
        }
    }
    
    printf("Maximum distance from centroid: %.6f\n", max_distance_from_center);
    
    return 0;
}