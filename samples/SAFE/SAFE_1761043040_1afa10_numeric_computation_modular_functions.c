//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
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
    printf("Enter number of points (max %d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    if (count <= 0 || count > max_count) {
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

int find_closest_pair(struct Point points[], int count, int *idx1, int *idx2) {
    if (count < 2) {
        return -1;
    }
    double min_dist = -1.0;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (min_dist < 0 || dist < min_dist) {
                min_dist = dist;
                *idx1 = i;
                *idx2 = j;
            }
        }
    }
    return 0;
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
        printf("Invalid input.\n");
        return 1;
    }
    int idx1, idx2;
    if (find_closest_pair(points, count, &idx1, &idx2) == 0) {
        double dist = calculate_distance(points[idx1], points[idx2]);
        printf("Closest points: (%g, %g) and (%g, %g)\n", 
               points[idx1].x, points[idx1].y, points[idx2].x, points[idx2].y);
        printf("Distance: %g\n", dist);
    } else {
        printf("Not enough points to find closest pair.\n");
    }
    double centroid_x = calculate_centroid_x(points, count);
    double centroid_y = calculate_centroid_y(points, count);
    printf("Centroid: (%g, %g)\n", centroid_x, centroid_y);
    return 0;
}