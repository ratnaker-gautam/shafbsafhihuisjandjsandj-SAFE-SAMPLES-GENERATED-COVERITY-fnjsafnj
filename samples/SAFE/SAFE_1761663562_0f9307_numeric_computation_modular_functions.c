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
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 1; i < count; i++) {
        total += calculate_distance(points[i-1], points[i]);
    }
    return total;
}

double calculate_area(struct Point points[], int count) {
    if (count < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;
        area += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    return fabs(area) / 2.0;
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
    
    int point_count = read_points(points, MAX_POINTS);
    if (point_count <= 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double total_dist = calculate_total_distance(points, point_count);
    double area = calculate_area(points, point_count);
    calculate_centroid(points, point_count, &centroid);
    
    printf("\nResults:\n");
    printf("Number of points: %d\n", point_count);
    printf("Total path distance: %.3f\n", total_dist);
    printf("Polygon area: %.3f\n", area);
    printf("Centroid: (%.3f, %.3f)\n", centroid.x, centroid.y);
    
    printf("\nPoint distances:\n");
    for (int i = 0; i < point_count - 1; i++) {
        double dist = calculate_distance(points[i], points[i+1]);
        printf("Distance from point %d to %d: %.3f\n", i+1, i+2, dist);
    }
    
    return 0;
}