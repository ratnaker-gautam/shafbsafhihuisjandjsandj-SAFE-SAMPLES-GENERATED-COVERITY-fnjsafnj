//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

double calculate_total_distance(struct Point points[], int count) {
    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        total += calculate_distance(points[i], points[i + 1]);
    }
    return total;
}

void find_closest_points(struct Point points[], int count, int *idx1, int *idx2) {
    double min_distance = -1.0;
    *idx1 = -1;
    *idx2 = -1;
    
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (*idx1 == -1 || dist < min_distance) {
                min_distance = dist;
                *idx1 = i;
                *idx2 = j;
            }
        }
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    int point_count;
    
    point_count = read_points(points, MAX_POINTS);
    if (point_count == -1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double total_distance = calculate_total_distance(points, point_count);
    printf("Total distance along path: %.3f\n", total_distance);
    
    int idx1, idx2;
    find_closest_points(points, point_count, &idx1, &idx2);
    printf("Closest points: (%d, %d) and (%d, %d)\n", 
           idx1 + 1, idx2 + 1, idx1 + 1, idx2 + 1);
    
    double closest_dist = calculate_distance(points[idx1], points[idx2]);
    printf("Distance between closest points: %.3f\n", closest_dist);
    
    return 0;
}