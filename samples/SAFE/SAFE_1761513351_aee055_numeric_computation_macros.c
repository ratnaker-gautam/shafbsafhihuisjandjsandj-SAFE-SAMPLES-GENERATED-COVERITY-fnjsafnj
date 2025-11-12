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

int main() {
    int num_points;
    Point points[MAX_POINTS];
    double total_distance = 0.0;
    
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
            fprintf(stderr, "Error: Invalid coordinates\n");
            return 1;
        }
        
        if (!isfinite(points[i].x) || !isfinite(points[i].y)) {
            fprintf(stderr, "Error: Coordinates must be finite numbers\n");
            return 1;
        }
    }
    
    for (int i = 0; i < num_points - 1; i++) {
        double segment_distance = DISTANCE(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
        if (!isfinite(segment_distance)) {
            fprintf(stderr, "Error: Distance calculation overflow\n");
            return 1;
        }
        total_distance += segment_distance;
        if (!isfinite(total_distance)) {
            fprintf(stderr, "Error: Total distance overflow\n");
            return 1;
        }
    }
    
    printf("Total path distance: %.6f\n", total_distance);
    
    double min_x = points[0].x;
    double max_x = points[0].x;
    double min_y = points[0].y;
    double max_y = points[0].y;
    
    for (int i = 1; i < num_points; i++) {
        if (points[i].x < min_x) min_x = points[i].x;
        if (points[i].x > max_x) max_x = points[i].x;
        if (points[i].y < min_y) min_y = points[i].y;
        if (points[i].y > max_y) max_y = points[i].y;
    }
    
    double bounding_width = max_x - min_x;
    double bounding_height = max_y - min_y;
    
    if (!isfinite(bounding_width) || !isfinite(bounding_height)) {
        fprintf(stderr, "Error: Bounding box calculation overflow\n");
        return 1;
    }
    
    printf("Bounding box: width=%.6f, height=%.6f\n", bounding_width, bounding_height);
    
    double center_x = (min_x + max_x) / 2.0;
    double center_y = (min_y + max_y) / 2.0;
    
    if (!isfinite(center_x) || !isfinite(center_y)) {
        fprintf(stderr, "Error: Center calculation overflow\n");
        return 1;
    }
    
    printf("Center point: (%.6f, %.6f)\n", center_x, center_y);
    
    return 0;
}