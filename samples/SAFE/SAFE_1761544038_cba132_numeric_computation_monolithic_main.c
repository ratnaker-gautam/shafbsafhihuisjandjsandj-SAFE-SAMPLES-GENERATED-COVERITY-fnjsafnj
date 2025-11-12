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
    
    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < num_points; i++) {
        printf("Point %d: ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid point coordinates\n");
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
    
    int pair_count = num_points * (num_points - 1) / 2;
    double average_distance = total_distance / pair_count;
    
    printf("\nDistance Statistics:\n");
    printf("Total pairs: %d\n", pair_count);
    printf("Minimum distance: %.6f\n", min_distance);
    printf("Maximum distance: %.6f\n", max_distance);
    printf("Average distance: %.6f\n", average_distance);
    
    double path_length = 0.0;
    for (int i = 0; i < num_points - 1; i++) {
        path_length += calculate_distance(points[i], points[i + 1]);
    }
    
    printf("Path length through sequential points: %.6f\n", path_length);
    
    double centroid_x = 0.0;
    double centroid_y = 0.0;
    for (int i = 0; i < num_points; i++) {
        centroid_x += points[i].x;
        centroid_y += points[i].y;
    }
    centroid_x /= num_points;
    centroid_y /= num_points;
    
    printf("Centroid: (%.6f, %.6f)\n", centroid_x, centroid_y);
    
    return 0;
}