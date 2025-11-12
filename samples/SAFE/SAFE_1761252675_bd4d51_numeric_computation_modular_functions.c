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

double calculate_distance(struct Point a, struct Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
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

struct Point calculate_centroid(struct Point points[], int count) {
    struct Point centroid = {0.0, 0.0};
    if (count < 1) {
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

void print_results(struct Point points[], int count, double total_dist, struct Point centroid, double area) {
    printf("\nResults:\n");
    printf("Number of points: %d\n", count);
    printf("Total distance between consecutive points: %.3f\n", total_dist);
    printf("Centroid: (%.3f, %.3f)\n", centroid.x, centroid.y);
    printf("Polygon area: %.3f\n", area);
    
    printf("\nPoint distances from centroid:\n");
    for (int i = 0; i < count; i++) {
        double dist = calculate_distance(points[i], centroid);
        printf("Point %d: %.3f\n", i + 1, dist);
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    double total_dist = calculate_total_distance(points, count);
    struct Point centroid = calculate_centroid(points, count);
    double area = calculate_area(points, count);
    
    print_results(points, count, total_dist, centroid, area);
    
    return 0;
}