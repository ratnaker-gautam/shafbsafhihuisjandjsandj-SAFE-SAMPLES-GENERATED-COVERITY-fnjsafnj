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

double calculate_distance(struct Point a, struct Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

double find_min_distance(struct Point points[], int count) {
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

double find_max_distance(struct Point points[], int count) {
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

struct Point calculate_centroid(struct Point points[], int count) {
    struct Point centroid = {0.0, 0.0};
    if (count == 0) {
        return centroid;
    }
    
    for (int i = 0; i < count; i++) {
        centroid.x += points[i].x;
        centroid.y += points[i].y;
    }
    centroid.x /= count;
    centroid.y /= count;
    return centroid;
}

void print_points(struct Point points[], int count) {
    printf("Points:\n");
    for (int i = 0; i < count; i++) {
        printf("  Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input or too many points.\n");
        return 1;
    }
    
    print_points(points, count);
    
    if (count >= 2) {
        double min_dist = find_min_distance(points, count);
        double max_dist = find_max_distance(points, count);
        printf("Minimum distance between points: %.4f\n", min_dist);
        printf("Maximum distance between points: %.4f\n", max_dist);
    }
    
    struct Point centroid = calculate_centroid(points, count);
    printf("Centroid of points: (%.4f, %.4f)\n", centroid.x, centroid.y);
    
    return 0;
}