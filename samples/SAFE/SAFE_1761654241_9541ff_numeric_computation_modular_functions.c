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
    if (fabs(coord) > 1e100) {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    
    if (fabs(dx) > sqrt(DBL_MAX) || fabs(dy) > sqrt(DBL_MAX)) {
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
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            return -1;
        }
        
        if (!validate_coordinate(x) || !validate_coordinate(y)) {
            return -1;
        }
        
        points[i].x = x;
        points[i].y = y;
    }
    
    return count;
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count < 0) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    double min_distance = find_minimum_distance(points, count);
    
    if (min_distance == DBL_MAX) {
        fprintf(stderr, "Error: Distance calculation overflow\n");
        return 1;
    }
    
    printf("Minimum distance between any two points: %.6f\n", min_distance);
    
    return 0;
}