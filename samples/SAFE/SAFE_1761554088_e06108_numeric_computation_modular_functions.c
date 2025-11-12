//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_POINTS 100

struct Point {
    double x;
    double y;
};

int validate_input_count(int count) {
    if (count <= 0 || count > MAX_POINTS) {
        return 0;
    }
    return 1;
}

int read_points(struct Point* points, int count) {
    for (int i = 0; i < count; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return 0;
        }
    }
    return 1;
}

double calculate_distance(struct Point a, struct Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

double find_minimum_distance(struct Point* points, int count) {
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

double find_maximum_distance(struct Point* points, int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double max_dist = 0.0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (dist > max_dist) {
                max_dist = dist;
            }
        }
    }
    
    return max_dist;
}

double calculate_centroid_x(struct Point* points, int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += points[i].x;
    }
    return sum / count;
}

double calculate_centroid_y(struct Point* points, int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += points[i].y;
    }
    return sum / count;
}

int main(void) {
    int point_count;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    if (!validate_input_count(point_count)) {
        printf("Invalid number of points.\n");
        return 1;
    }
    
    struct Point points[MAX_POINTS];
    
    printf("Enter %d points as x y coordinates:\n", point_count);
    if (!read_points(points, point_count)) {
        printf("Failed to read points.\n");
        return 1;
    }
    
    if (point_count == 1) {
        printf("Single point at (%.2f, %.2f)\n", points[0].x, points[0].y);
        return 0;
    }
    
    double min_dist = find_minimum_distance(points, point_count);
    double max_dist = find_maximum_distance(points, point_count);
    double centroid_x = calculate_centroid_x(points, point_count);
    double centroid_y = calculate_centroid_y(points, point_count);
    
    printf("Minimum distance between points: %.4f\n", min_dist);
    printf("Maximum distance between points: %.4f\n", max_dist);
    printf("Centroid of points: (%.4f, %.4f)\n", centroid_x, centroid_y);
    
    return 0;
}