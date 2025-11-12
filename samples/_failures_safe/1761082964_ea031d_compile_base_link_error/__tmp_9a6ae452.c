//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_POINTS 100
#define PI 3.14159265358979323846
#define SQUARE(x) ((x) * (x))
#define RADIANS(degrees) ((degrees) * PI / 180.0)
#define DISTANCE(x1, y1, x2, y2) sqrt(SQUARE((x2) - (x1)) + SQUARE((y2) - (y1)))
#define AREA_CIRCLE(radius) (PI * SQUARE(radius))
#define VOLUME_SPHERE(radius) (4.0 / 3.0 * PI * SQUARE(radius) * (radius))

struct Point {
    double x;
    double y;
};

int main(void) {
    int num_points;
    struct Point points[MAX_POINTS];
    double total_distance = 0.0;
    double min_x = 0.0, max_x = 0.0, min_y = 0.0, max_y = 0.0;
    int first_point = 1;

    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }

    if (num_points < 2 || num_points > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return EXIT_FAILURE;
    }

    printf("Enter coordinates for %d points (x y):\n", num_points);
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return EXIT_FAILURE;
        }

        if (first_point) {
            min_x = points[i].x;
            max_x = points[i].x;
            min_y = points[i].y;
            max_y = points[i].y;
            first_point = 0;
        } else {
            if (points[i].x < min_x) min_x = points[i].x;
            if (points[i].x > max_x) max_x = points[i].x;
            if (points[i].y < min_y) min_y = points[i].y;
            if (points[i].y > max_y) max_y = points[i].y;
        }
    }

    for (int i = 0; i < num_points - 1; i++) {
        double dist = DISTANCE(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
        total_distance += dist;
    }

    double center_x = (min_x + max_x) / 2.0;
    double center_y = (min_y + max_y) / 2.0;
    double bounding_radius = DISTANCE(center_x, center_y, max_x, max_y);
    double bounding_area = AREA_CIRCLE(bounding_radius);
    double bounding_volume = VOLUME_SPHERE(bounding_radius);

    printf("\nResults:\n");
    printf("Total path distance: %.6f\n", total_distance);
    printf("Bounding circle center: (%.6f, %.6f)\n", center_x, center_y);
    printf("Bounding circle radius: %.6f\n", bounding_radius);
    printf("Bounding circle area: %.6f\n", bounding_area);
    printf("Bounding sphere volume: %.6f\n", bounding_volume);

    double angle = 45.0;
    double radians = RADIANS(angle);
    printf("Example: sin(%.1f°) = %.6f\n", angle, sin(radians));

    return EXIT_SUCCESS;
}