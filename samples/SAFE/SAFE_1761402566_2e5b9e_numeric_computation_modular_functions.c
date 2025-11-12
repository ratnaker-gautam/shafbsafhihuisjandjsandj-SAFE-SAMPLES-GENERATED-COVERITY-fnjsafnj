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
        if (!isfinite(points[i].x) || !isfinite(points[i].y)) {
            return 0;
        }
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double distance_squared = dx * dx + dy * dy;
    if (distance_squared < 0.0 || !isfinite(distance_squared)) {
        return -1.0;
    }
    return sqrt(distance_squared);
}

double find_minimum_distance(struct Point* points, int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double min_distance = -1.0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double distance = calculate_distance(points[i], points[j]);
            if (distance < 0.0) {
                return -1.0;
            }
            if (min_distance < 0.0 || distance < min_distance) {
                min_distance = distance;
            }
        }
    }
    
    return min_distance;
}

double calculate_centroid_x(struct Point* points, int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        if (!isfinite(sum + points[i].x)) {
            return 0.0;
        }
        sum += points[i].x;
    }
    return sum / count;
}

double calculate_centroid_y(struct Point* points, int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        if (!isfinite(sum + points[i].y)) {
            return 0.0;
        }
        sum += points[i].y;
    }
    return sum / count;
}

int main(void) {
    int point_count;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (!validate_input_count(point_count)) {
        printf("Invalid point count\n");
        return 1;
    }
    
    struct Point points[MAX_POINTS];
    
    printf("Enter %d points as 'x y' pairs:\n", point_count);
    if (!read_points(points, point_count)) {
        printf("Invalid point data\n");
        return 1;
    }
    
    double min_distance = find_minimum_distance(points, point_count);
    if (min_distance < 0.0) {
        printf("Error calculating distances\n");
        return 1;
    }
    
    double centroid_x = calculate_centroid_x(points, point_count);
    double centroid_y = calculate_centroid_y(points, point_count);
    
    if (!isfinite(centroid_x) || !isfinite(centroid_y)) {
        printf("Error calculating centroid\n");
        return 1;
    }
    
    printf("Minimum distance between points: %.6f\n", min_distance);
    printf("Centroid of points: (%.6f, %.6f)\n", centroid_x, centroid_y);
    
    return 0;
}