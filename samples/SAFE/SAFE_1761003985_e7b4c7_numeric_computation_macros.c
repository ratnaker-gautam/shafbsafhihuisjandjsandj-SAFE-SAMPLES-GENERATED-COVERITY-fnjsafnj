//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_POINTS 100
#define PI 3.14159265358979323846
#define SQUARE(x) ((x) * (x))
#define RADIANS(degrees) ((degrees) * PI / 180.0)
#define IS_VALID_INDEX(i, size) ((i) >= 0 && (i) < (size))

typedef struct {
    double x;
    double y;
} Point;

double calculate_distance(Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(SQUARE(dx) + SQUARE(dy));
}

double calculate_polygon_area(Point points[], int num_points) {
    if (num_points < 3) return 0.0;
    double area = 0.0;
    for (int i = 0; i < num_points; i++) {
        int j = (i + 1) % num_points;
        area += points[i].x * points[j].y;
        area -= points[j].x * points[i].y;
    }
    return fabs(area) / 2.0;
}

int main() {
    int num_points;
    printf("Enter number of points in polygon (3-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    if (num_points < 3 || num_points > MAX_POINTS) {
        printf("Number of points must be between 3 and %d.\n", MAX_POINTS);
        return 1;
    }

    Point points[MAX_POINTS];
    printf("Enter coordinates for %d points (x y):\n", num_points);
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            printf("Invalid coordinate input.\n");
            return 1;
        }
    }

    double perimeter = 0.0;
    for (int i = 0; i < num_points; i++) {
        int j = (i + 1) % num_points;
        perimeter += calculate_distance(points[i], points[j]);
    }

    double area = calculate_polygon_area(points, num_points);

    printf("Polygon properties:\n");
    printf("Perimeter: %.6f\n", perimeter);
    printf("Area: %.6f\n", area);

    double min_distance = -1.0;
    double max_distance = 0.0;
    for (int i = 0; i < num_points; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
            }
            if (dist > max_distance) {
                max_distance = dist;
            }
        }
    }

    printf("Minimum distance between points: %.6f\n", min_distance);
    printf("Maximum distance between points: %.6f\n", max_distance);

    return 0;
}