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

int main() {
    int num_points;
    double points[MAX_POINTS][2];
    double total_distance = 0.0;
    double min_x = 0.0, max_x = 0.0, min_y = 0.0, max_y = 0.0;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (num_points < 2 || num_points > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return 1;
    }
    
    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < num_points; i++) {
        printf("Point %d: ", i + 1);
        if (scanf("%lf %lf", &points[i][0], &points[i][1]) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return 1;
        }
        
        if (i == 0) {
            min_x = points[i][0];
            max_x = points[i][0];
            min_y = points[i][1];
            max_y = points[i][1];
        } else {
            if (points[i][0] < min_x) min_x = points[i][0];
            if (points[i][0] > max_x) max_x = points[i][0];
            if (points[i][1] < min_y) min_y = points[i][1];
            if (points[i][1] > max_y) max_y = points[i][1];
        }
    }
    
    for (int i = 0; i < num_points - 1; i++) {
        double dist = DISTANCE(points[i][0], points[i][1], points[i+1][0], points[i+1][1]);
        if (dist < 0 || !isfinite(dist)) {
            fprintf(stderr, "Error: Invalid distance calculation\n");
            return 1;
        }
        total_distance += dist;
    }
    
    double bounding_width = max_x - min_x;
    double bounding_height = max_y - min_y;
    double bounding_area = bounding_width * bounding_height;
    
    if (bounding_area < 0 || !isfinite(bounding_area)) {
        fprintf(stderr, "Error: Invalid bounding area calculation\n");
        return 1;
    }
    
    printf("\nPath Statistics:\n");
    printf("Total distance: %.6f\n", total_distance);
    printf("Bounding box: (%.6f, %.6f) to (%.6f, %.6f)\n", min_x, min_y, max_x, max_y);
    printf("Bounding area: %.6f\n", bounding_area);
    
    double center_x = (min_x + max_x) / 2.0;
    double center_y = (min_y + max_y) / 2.0;
    printf("Center point: (%.6f, %.6f)\n", center_x, center_y);
    
    double max_distance_from_center = 0.0;
    for (int i = 0; i < num_points; i++) {
        double dist = DISTANCE(points[i][0], points[i][1], center_x, center_y);
        if (dist > max_distance_from_center) {
            max_distance_from_center = dist;
        }
    }
    
    printf("Maximum distance from center: %.6f\n", max_distance_from_center);
    
    return 0;
}