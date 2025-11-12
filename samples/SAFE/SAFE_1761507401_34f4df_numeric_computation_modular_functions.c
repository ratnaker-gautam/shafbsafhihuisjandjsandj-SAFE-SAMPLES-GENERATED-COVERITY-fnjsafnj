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
    for (int i = 0; i < count - 1; i++) {
        total += calculate_distance(points[i], points[i + 1]);
    }
    return total;
}

void calculate_centroid(struct Point points[], int count, double *centroid_x, double *centroid_y) {
    if (count <= 0) {
        *centroid_x = 0.0;
        *centroid_y = 0.0;
        return;
    }
    double sum_x = 0.0;
    double sum_y = 0.0;
    for (int i = 0; i < count; i++) {
        sum_x += points[i].x;
        sum_y += points[i].y;
    }
    *centroid_x = sum_x / count;
    *centroid_y = sum_y / count;
}

int main() {
    struct Point points[MAX_POINTS];
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input.\n");
        return 1;
    }
    double total_dist = calculate_total_distance(points, count);
    double centroid_x, centroid_y;
    calculate_centroid(points, count, &centroid_x, &centroid_y);
    printf("Total distance between consecutive points: %.3f\n", total_dist);
    printf("Centroid: (%.3f, %.3f)\n", centroid_x, centroid_y);
    return 0;
}