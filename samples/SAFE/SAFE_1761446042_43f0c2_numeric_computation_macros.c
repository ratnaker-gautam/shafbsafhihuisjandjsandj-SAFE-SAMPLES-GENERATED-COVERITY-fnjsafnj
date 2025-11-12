//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_POINTS 100
#define PI 3.14159265358979323846
#define SQUARE(x) ((x) * (x))
#define RADIANS(degrees) ((degrees) * PI / 180.0)
#define DISTANCE(x1, y1, x2, y2) (sqrt(SQUARE((x2) - (x1)) + SQUARE((y2) - (y1))))

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

double calculate_polygon_perimeter(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double perimeter = 0.0;
    for (int i = 0; i < count; i++) {
        int next = (i + 1) % count;
        perimeter += DISTANCE(points[i].x, points[i].y, points[next].x, points[next].y);
    }
    return perimeter;
}

double calculate_polygon_area(struct Point points[], int count) {
    if (count < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    for (int i = 0; i < count; i++) {
        int next = (i + 1) % count;
        area += (points[i].x * points[next].y - points[next].x * points[i].y);
    }
    return fabs(area) / 2.0;
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    double perimeter = calculate_polygon_perimeter(points, count);
    double area = calculate_polygon_area(points, count);
    
    printf("Polygon with %d points:\n", count);
    printf("Perimeter: %.6f\n", perimeter);
    printf("Area: %.6f\n", area);
    
    return 0;
}