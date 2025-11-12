//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    if (coord < -1e6 || coord > 1e6) {
        return 0;
    }
    if (coord != coord) {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    
    if (dx > 1e150 || dy > 1e150 || dx < -1e150 || dy < -1e150) {
        return -1.0;
    }
    
    double squared = dx * dx + dy * dy;
    if (squared > 1e300) {
        return -1.0;
    }
    
    return sqrt(squared);
}

double find_minimum_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double min_distance = -1.0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double distance = calculate_distance(points[i], points[j]);
            if (distance >= 0.0) {
                if (min_distance < 0.0 || distance < min_distance) {
                    min_distance = distance;
                }
            }
        }
    }
    
    return min_distance;
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
    
    int point_count = read_points(points, MAX_POINTS);
    if (point_count < 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double min_distance = find_minimum_distance(points, point_count);
    
    if (min_distance < 0.0) {
        printf("Error: Could not calculate minimum distance\n");
        return 1;
    }
    
    printf("Minimum distance between any two points: %.6f\n", min_distance);
    
    return 0;
}