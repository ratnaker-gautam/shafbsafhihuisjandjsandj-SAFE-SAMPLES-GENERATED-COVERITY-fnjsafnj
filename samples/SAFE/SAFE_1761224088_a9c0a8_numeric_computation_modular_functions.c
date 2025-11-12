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

double calculate_perimeter(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double perimeter = 0.0;
    for (int i = 0; i < count; i++) {
        int next = (i + 1) % count;
        perimeter += calculate_distance(points[i], points[next]);
    }
    return perimeter;
}

double calculate_area(struct Point points[], int count) {
    if (count < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    for (int i = 0; i < count; i++) {
        int next = (i + 1) % count;
        area += points[i].x * points[next].y - points[next].x * points[i].y;
    }
    return fabs(area) / 2.0;
}

void calculate_centroid(struct Point points[], int count, struct Point *centroid) {
    centroid->x = 0.0;
    centroid->y = 0.0;
    if (count < 1) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        centroid->x += points[i].x;
        centroid->y += points[i].y;
    }
    centroid->x /= count;
    centroid->y /= count;
}

int main(void) {
    struct Point points[MAX_POINTS];
    struct Point centroid;
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    double perimeter = calculate_perimeter(points, count);
    double area = calculate_area(points, count);
    calculate_centroid(points, count, &centroid);
    
    printf("Polygon with %d points:\n", count);
    printf("Perimeter: %.3f\n", perimeter);
    printf("Area: %.3f\n", area);
    printf("Centroid: (%.3f, %.3f)\n", centroid.x, centroid.y);
    
    return 0;
}