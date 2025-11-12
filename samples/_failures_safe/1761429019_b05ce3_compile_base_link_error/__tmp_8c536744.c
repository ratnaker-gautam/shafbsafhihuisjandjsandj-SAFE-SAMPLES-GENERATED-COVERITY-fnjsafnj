//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_POINTS 100
#define PI 3.14159265358979323846
#define SQUARE(x) ((x) * (x))
#define RADIANS(deg) ((deg) * PI / 180.0)
#define IS_VALID_ANGLE(a) ((a) >= 0.0 && (a) <= 360.0)

typedef struct {
    double x;
    double y;
} Point;

int main() {
    int num_points;
    Point points[MAX_POINTS];
    double angle;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (num_points < 1 || num_points > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 1 and %d\n", MAX_POINTS);
        return 1;
    }
    
    printf("Enter rotation angle in degrees (0-360): ");
    if (scanf("%lf", &angle) != 1) {
        fprintf(stderr, "Error: Invalid angle input\n");
        return 1;
    }
    
    if (!IS_VALID_ANGLE(angle)) {
        fprintf(stderr, "Error: Angle must be between 0 and 360 degrees\n");
        return 1;
    }
    
    printf("Enter %d points (x y coordinates):\n", num_points);
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid point coordinates\n");
            return 1;
        }
    }
    
    double rad_angle = RADIANS(angle);
    double cos_angle = cos(rad_angle);
    double sin_angle = sin(rad_angle);
    
    printf("\nOriginal points:\n");
    for (int i = 0; i < num_points; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    
    printf("\nRotated points (%.1f degrees):\n", angle);
    for (int i = 0; i < num_points; i++) {
        double new_x = points[i].x * cos_angle - points[i].y * sin_angle;
        double new_y = points[i].x * sin_angle + points[i].y * cos_angle;
        
        if (fabs(new_x) < 1e-10) new_x = 0.0;
        if (fabs(new_y) < 1e-10) new_y = 0.0;
        
        printf("Point %d: (%.2f, %.2f)\n", i + 1, new_x, new_y);
    }
    
    double total_distance = 0.0;
    if (num_points > 1) {
        for (int i = 0; i < num_points - 1; i++) {
            double dx = points[i + 1].x - points[i].x;
            double dy = points[i + 1].y - points[i].y;
            double distance = sqrt(SQUARE(dx) + SQUARE(dy));
            if (!isnan(distance) && !isinf(distance)) {
                total_distance += distance;
            }
        }
        printf("\nTotal distance between consecutive points: %.2f\n", total_distance);
    }
    
    return 0;
}