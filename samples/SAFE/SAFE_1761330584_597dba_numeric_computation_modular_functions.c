//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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

int validate_coordinate(double coord) {
    if (isnan(coord) || isinf(coord)) {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    if (dx > sqrt(DBL_MAX) || dy > sqrt(DBL_MAX)) {
        return DBL_MAX;
    }
    double dx_sq = dx * dx;
    double dy_sq = dy * dy;
    if (dx_sq > DBL_MAX - dy_sq) {
        return DBL_MAX;
    }
    return sqrt(dx_sq + dy_sq);
}

double find_minimum_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double min_dist = DBL_MAX;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (dist < min_dist) {
                min_dist = dist;
            }
        }
    }
    return min_dist;
}

int read_points(struct Point points[], int max_count) {
    int count;
    printf("Enter number of points (2-%d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    
    if (!validate_input_count(count)) {
        return -1;
    }
    
    printf("Enter %d points as x y coordinates:\n", count);
    for (int i = 0; i < count; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return -1;
        }
        if (!validate_coordinate(points[i].x) || !validate_coordinate(points[i].y)) {
            return -1;
        }
    }
    
    return count;
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int point_count = read_points(points, MAX_POINTS);
    if (point_count < 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double min_distance = find_minimum_distance(points, point_count);
    
    if (min_distance == DBL_MAX) {
        printf("Error: Distance calculation overflow\n");
        return 1;
    }
    
    printf("Minimum distance between any two points: %.6f\n", min_distance);
    
    return 0;
}