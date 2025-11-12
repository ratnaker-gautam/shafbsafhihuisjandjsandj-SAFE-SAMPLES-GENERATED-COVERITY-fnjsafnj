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
    for (int i = 0; i < count - 1; i++) {
        total += calculate_distance(points[i], points[i + 1]);
    }
    return total;
}

void calculate_centroid(struct Point points[], int count, struct Point *centroid) {
    if (count < 1) {
        centroid->x = 0.0;
        centroid->y = 0.0;
        return;
    }
    
    double sum_x = 0.0;
    double sum_y = 0.0;
    
    for (int i = 0; i < count; i++) {
        sum_x += points[i].x;
        sum_y += points[i].y;
    }
    
    centroid->x = sum_x / count;
    centroid->y = sum_y / count;
}

int main(void) {
    struct Point points[MAX_POINTS];
    struct Point centroid;
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    double total_distance = calculate_total_distance(points, count);
    calculate_centroid(points, count, &centroid);
    
    printf("\nResults:\n");
    printf("Number of points: %d\n", count);
    printf("Total distance between consecutive points: %.3f\n", total_distance);
    printf("Centroid: (%.3f, %.3f)\n", centroid.x, centroid.y);
    
    return 0;
}