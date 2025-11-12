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
    if (count < 1 || count > max_count) {
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

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 1; i < count; i++) {
        total += calculate_distance(points[i - 1], points[i]);
    }
    return total;
}

void calculate_statistics(struct Point points[], int count, double *min_dist, double *max_dist) {
    if (count < 2) {
        *min_dist = 0.0;
        *max_dist = 0.0;
        return;
    }
    
    *min_dist = calculate_distance(points[0], points[1]);
    *max_dist = *min_dist;
    
    for (int i = 1; i < count - 1; i++) {
        double dist = calculate_distance(points[i], points[i + 1]);
        if (dist < *min_dist) {
            *min_dist = dist;
        }
        if (dist > *max_dist) {
            *max_dist = dist;
        }
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double total_distance = calculate_total_distance(points, count);
    double min_distance, max_distance;
    calculate_statistics(points, count, &min_distance, &max_distance);
    
    printf("\nResults for %d points:\n", count);
    printf("Total path distance: %.3f\n", total_distance);
    printf("Minimum segment distance: %.3f\n", min_distance);
    printf("Maximum segment distance: %.3f\n", max_distance);
    
    if (count >= 2) {
        printf("Average segment distance: %.3f\n", total_distance / (count - 1));
    }
    
    return 0;
}