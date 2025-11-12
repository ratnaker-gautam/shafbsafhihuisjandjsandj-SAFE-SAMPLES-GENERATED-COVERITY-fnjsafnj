//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100

struct Point {
    double x;
    double y;
};

int read_points(struct Point points[], int max_count) {
    int count;
    
    printf("Enter number of points (1-%d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    
    if (count <= 0 || count > max_count) {
        return -1;
    }
    
    printf("Enter %d points as x y coordinates:\n", count);
    for (int i = 0; i < count; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return -1;
        }
    }
    
    return count;
}

double calculate_distance(struct Point a, struct Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

double find_minimum_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double min_dist = -1.0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (min_dist < 0 || dist < min_dist) {
                min_dist = dist;
            }
        }
    }
    
    return min_dist;
}

double calculate_centroid_x(struct Point points[], int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += points[i].x;
    }
    return sum / count;
}

double calculate_centroid_y(struct Point points[], int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += points[i].y;
    }
    return sum / count;
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double min_distance = find_minimum_distance(points, count);
    double centroid_x = calculate_centroid_x(points, count);
    double centroid_y = calculate_centroid_y(points, count);
    
    printf("Number of points: %d\n", count);
    printf("Minimum distance between any two points: %.6f\n", min_distance);
    printf("Centroid coordinates: (%.6f, %.6f)\n", centroid_x, centroid_y);
    
    double total_distance_to_centroid = 0.0;
    for (int i = 0; i < count; i++) {
        struct Point centroid = {centroid_x, centroid_y};
        total_distance_to_centroid += calculate_distance(points[i], centroid);
    }
    double avg_distance_to_centroid = total_distance_to_centroid / count;
    printf("Average distance to centroid: %.6f\n", avg_distance_to_centroid);
    
    return 0;
}