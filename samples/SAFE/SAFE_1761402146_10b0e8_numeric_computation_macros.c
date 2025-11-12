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

typedef struct {
    double x;
    double y;
} Point;

int main(void) {
    int num_points;
    Point points[MAX_POINTS];
    double total_distance = 0.0;
    double min_distance = __DBL_MAX__;
    double max_distance = 0.0;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (num_points < 2 || num_points > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return EXIT_FAILURE;
    }
    
    printf("Enter coordinates for %d points:\n", num_points);
    for (int i = 0; i < num_points; i++) {
        printf("Point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinates\n");
            return EXIT_FAILURE;
        }
        
        if (!isfinite(points[i].x) || !isfinite(points[i].y)) {
            fprintf(stderr, "Error: Coordinates must be finite numbers\n");
            return EXIT_FAILURE;
        }
    }
    
    for (int i = 0; i < num_points - 1; i++) {
        double dist = DISTANCE(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
        
        if (!isfinite(dist) || dist < 0.0) {
            fprintf(stderr, "Error: Invalid distance calculation\n");
            return EXIT_FAILURE;
        }
        
        total_distance += dist;
        
        if (dist < min_distance) {
            min_distance = dist;
        }
        
        if (dist > max_distance) {
            max_distance = dist;
        }
    }
    
    double avg_distance = total_distance / (num_points - 1);
    
    printf("\nPath Statistics:\n");
    printf("Total distance: %.6f\n", total_distance);
    printf("Average distance between points: %.6f\n", avg_distance);
    printf("Minimum distance between consecutive points: %.6f\n", min_distance);
    printf("Maximum distance between consecutive points: %.6f\n", max_distance);
    
    double polygon_area = 0.0;
    for (int i = 0; i < num_points; i++) {
        int j = (i + 1) % num_points;
        polygon_area += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    polygon_area = fabs(polygon_area) / 2.0;
    
    if (!isfinite(polygon_area)) {
        fprintf(stderr, "Error: Invalid area calculation\n");
        return EXIT_FAILURE;
    }
    
    printf("Polygon area: %.6f\n", polygon_area);
    
    double centroid_x = 0.0, centroid_y = 0.0;
    for (int i = 0; i < num_points; i++) {
        centroid_x += points[i].x;
        centroid_y += points[i].y;
    }
    centroid_x /= num_points;
    centroid_y /= num_points;
    
    printf("Centroid: (%.6f, %.6f)\n", centroid_x, centroid_y);
    
    return EXIT_SUCCESS;
}