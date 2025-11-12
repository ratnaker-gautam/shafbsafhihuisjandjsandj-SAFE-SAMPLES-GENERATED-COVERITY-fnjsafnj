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

    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid point coordinates\n");
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

    for (int i = 1; i < num_points; i++) {
        double dist = DISTANCE(points[i-1].x, points[i-1].y, points[i].x, points[i].y);
        if (dist < 0.0) {
            fprintf(stderr, "Error: Distance calculation overflow\n");
            return EXIT_FAILURE;
        }
        total_distance += dist;
    }

    double width = max_x - min_x;
    double height = max_y - min_y;
    double bounding_area = width * height;

    printf("\nPath Analysis Results:\n");
    printf("Total distance: %.6f\n", total_distance);
    printf("Bounding box: (%.6f, %.6f) to (%.6f, %.6f)\n", min_x, min_y, max_x, max_y);
    printf("Bounding area: %.6f\n", bounding_area);
    printf("Average segment length: %.6f\n", total_distance / (num_points - 1));

    double center_x = (min_x + max_x) / 2.0;
    double center_y = (min_y + max_y) / 2.0;
    printf("Center of bounding box: (%.6f, %.6f)\n", center_x, center_y);

    double max_distance_from_center = 0.0;
    for (int i = 0; i < num_points; i++) {
        double dist = DISTANCE(points[i].x, points[i].y, center_x, center_y);
        if (dist > max_distance_from_center) {
            max_distance_from_center = dist;
        }
    }
    printf("Maximum distance from center: %.6f\n", max_distance_from_center);

    return EXIT_SUCCESS;
}