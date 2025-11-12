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

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points(struct Point points[], int max_count) {
    int count;
    printf("Enter number of points (2-%d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    if (count < 2 || count > max_count) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        printf("Enter point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return -1;
        }
    }
    return count;
}

double find_minimum_distance(struct Point points[], int count) {
    double min_distance = -1.0;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
            }
        }
    }
    return min_distance;
}

double calculate_total_distance(struct Point points[], int count) {
    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        total += calculate_distance(points[i], points[i + 1]);
    }
    return total;
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int point_count = read_points(points, MAX_POINTS);
    if (point_count < 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double min_dist = find_minimum_distance(points, point_count);
    double total_dist = calculate_total_distance(points, point_count);
    
    printf("Minimum distance between any two points: %.3f\n", min_dist);
    printf("Total distance along sequence: %.3f\n", total_dist);
    
    double area = 0.0;
    for (int i = 0; i < point_count - 1; i++) {
        area += (points[i].x * points[i + 1].y - points[i + 1].x * points[i].y);
    }
    area = fabs(area) / 2.0;
    printf("Approximate polygon area: %.3f\n", area);
    
    return 0;
}