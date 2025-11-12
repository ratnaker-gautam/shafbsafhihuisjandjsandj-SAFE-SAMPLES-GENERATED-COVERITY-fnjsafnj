//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_POINTS 100
#define PI 3.14159265358979323846
#define SQUARE(x) ((x) * (x))
#define RADIANS(deg) ((deg) * PI / 180.0)
#define DISTANCE(x1, y1, x2, y2) (sqrt(SQUARE((x2) - (x1)) + SQUARE((y2) - (y1))))

int main() {
    int num_points;
    double points[MAX_POINTS][2];
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
    
    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < num_points; i++) {
        printf("Point %d: ", i + 1);
        if (scanf("%lf %lf", &points[i][0], &points[i][1]) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return 1;
        }
        
        if (!isfinite(points[i][0]) || !isfinite(points[i][1])) {
            fprintf(stderr, "Error: Coordinate values must be finite\n");
            return 1;
        }
    }
    
    for (int i = 0; i < num_points - 1; i++) {
        double dist = DISTANCE(points[i][0], points[i][1], points[i+1][0], points[i+1][1]);
        if (!isfinite(dist)) {
            fprintf(stderr, "Error: Distance calculation overflow\n");
            return 1;
        }
        total_distance += dist;
    }
    
    double angle = RADIANS(45.0);
    double scale_factor = 1.5;
    double transformed_points[MAX_POINTS][2];
    
    for (int i = 0; i < num_points; i++) {
        double x = points[i][0];
        double y = points[i][1];
        
        transformed_points[i][0] = (x * cos(angle) - y * sin(angle)) * scale_factor;
        transformed_points[i][1] = (x * sin(angle) + y * cos(angle)) * scale_factor;
        
        if (!isfinite(transformed_points[i][0]) || !isfinite(transformed_points[i][1])) {
            fprintf(stderr, "Error: Transformation resulted in invalid values\n");
            return 1;
        }
    }
    
    printf("\nOriginal path distance: %.6f\n", total_distance);
    
    double transformed_distance = 0.0;
    for (int i = 0; i < num_points - 1; i++) {
        double dist = DISTANCE(transformed_points[i][0], transformed_points[i][1], 
                             transformed_points[i+1][0], transformed_points[i+1][1]);
        if (!isfinite(dist)) {
            fprintf(stderr, "Error: Transformed distance calculation overflow\n");
            return 1;
        }
        transformed_distance += dist;
    }
    
    printf("Transformed path distance: %.6f\n", transformed_distance);
    printf("Scale ratio: %.6f\n", transformed_distance / total_distance);
    
    double area_sum = 0.0;
    for (int i = 0; i < num_points; i++) {
        int j = (i + 1) % num_points;
        double cross = points[i][0] * points[j][1] - points[j][0] * points[i][1];
        if (!isfinite(cross)) {
            fprintf(stderr, "Error: Area calculation overflow\n");
            return 1;
        }
        area_sum += cross;
    }
    
    double polygon_area = fabs(area_sum) / 2.0;
    printf("Polygon area: %.6f\n", polygon_area);
    
    return 0;
}