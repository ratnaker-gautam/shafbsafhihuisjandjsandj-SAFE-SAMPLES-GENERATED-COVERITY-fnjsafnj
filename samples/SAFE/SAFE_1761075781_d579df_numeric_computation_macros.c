//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_POINTS 100
#define PI 3.14159265358979323846
#define SQUARE(x) ((x) * (x))
#define RADIANS(degrees) ((degrees) * PI / 180.0)
#define DISTANCE(x1, y1, x2, y2) (sqrt(SQUARE((x2) - (x1)) + SQUARE((y2) - (y1))))

typedef struct {
    double x;
    double y;
} Point;

int main() {
    int num_points;
    Point points[MAX_POINTS];
    double total_distance = 0.0;
    double min_distance = __DBL_MAX__;
    double max_distance = 0.0;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (num_points < 2 || num_points > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return 1;
    }
    
    printf("Enter coordinates for %d points:\n", num_points);
    for (int i = 0; i < num_points; i++) {
        printf("Point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return 1;
        }
    }
    
    printf("\nCalculating distances between consecutive points:\n");
    for (int i = 0; i < num_points - 1; i++) {
        double dist = DISTANCE(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
        
        if (dist < 0.0) {
            fprintf(stderr, "Error: Invalid distance calculation\n");
            return 1;
        }
        
        total_distance += dist;
        
        if (dist < min_distance) {
            min_distance = dist;
        }
        
        if (dist > max_distance) {
            max_distance = dist;
        }
        
        printf("Distance from point %d to point %d: %.6f\n", i + 1, i + 2, dist);
    }
    
    printf("\nTotal path distance: %.6f\n", total_distance);
    printf("Minimum segment distance: %.6f\n", min_distance);
    printf("Maximum segment distance: %.6f\n", max_distance);
    
    if (num_points >= 3) {
        double area = 0.0;
        for (int i = 0; i < num_points; i++) {
            int j = (i + 1) % num_points;
            area += points[i].x * points[j].y - points[j].x * points[i].y;
        }
        area = fabs(area) / 2.0;
        printf("Polygon area: %.6f\n", area);
    }
    
    double center_x = 0.0;
    double center_y = 0.0;
    for (int i = 0; i < num_points; i++) {
        center_x += points[i].x;
        center_y += points[i].y;
    }
    center_x /= num_points;
    center_y /= num_points;
    
    printf("Centroid: (%.6f, %.6f)\n", center_x, center_y);
    
    return 0;
}