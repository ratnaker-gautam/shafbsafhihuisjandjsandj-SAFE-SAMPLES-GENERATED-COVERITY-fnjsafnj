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

void calculate_bounding_box(struct Point points[], int count, double *min_x, double *max_x, double *min_y, double *max_y) {
    if (count == 0) {
        *min_x = *max_x = *min_y = *max_y = 0.0;
        return;
    }
    
    *min_x = points[0].x;
    *max_x = points[0].x;
    *min_y = points[0].y;
    *max_y = points[0].y;
    
    for (int i = 1; i < count; i++) {
        if (points[i].x < *min_x) *min_x = points[i].x;
        if (points[i].x > *max_x) *max_x = points[i].x;
        if (points[i].y < *min_y) *min_y = points[i].y;
        if (points[i].y > *max_y) *max_y = points[i].y;
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("\nPoint Analysis Results:\n");
    printf("Number of points: %d\n", count);
    
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    
    double total_dist = calculate_total_distance(points, count);
    printf("Total distance between consecutive points: %.4f\n", total_dist);
    
    double min_x, max_x, min_y, max_y;
    calculate_bounding_box(points, count, &min_x, &max_x, &min_y, &max_y);
    printf("Bounding box: X:[%.2f, %.2f] Y:[%.2f, %.2f]\n", min_x, max_x, min_y, max_y);
    printf("Box width: %.2f, height: %.2f\n", max_x - min_x, max_y - min_y);
    
    return 0;
}