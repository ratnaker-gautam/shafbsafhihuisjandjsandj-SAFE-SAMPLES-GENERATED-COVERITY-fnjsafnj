//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
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
    
    printf("\nCalculating total path distance...\n");
    for (int i = 0; i < num_points - 1; i++) {
        double segment_distance = calculate_distance(points[i], points[i + 1]);
        total_distance += segment_distance;
        printf("Segment %d-%d: %.3f\n", i + 1, i + 2, segment_distance);
    }
    
    printf("\nTotal path distance: %.3f\n", total_distance);
    
    double average_x = 0.0, average_y = 0.0;
    for (int i = 0; i < num_points; i++) {
        average_x += points[i].x;
        average_y += points[i].y;
    }
    average_x /= num_points;
    average_y /= num_points;
    
    printf("Centroid of points: (%.3f, %.3f)\n", average_x, average_y);
    
    double max_distance = 0.0;
    int max_i = 0, max_j = 0;
    for (int i = 0; i < num_points; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (dist > max_distance) {
                max_distance = dist;
                max_i = i;
                max_j = j;
            }
        }
    }
    
    printf("Farthest points: %d and %d (distance: %.3f)\n", 
           max_i + 1, max_j + 1, max_distance);
    
    return 0;
}