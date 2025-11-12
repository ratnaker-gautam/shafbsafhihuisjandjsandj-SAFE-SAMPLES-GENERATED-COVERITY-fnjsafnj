//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_POINTS 100
#define SQUARE(x) ((x) * (x))
#define DISTANCE(x1, y1, x2, y2) (sqrt(SQUARE((x1) - (x2)) + SQUARE((y1) - (y2))))
#define IS_VALID_COORDINATE(x) ((x) >= -1000.0 && (x) <= 1000.0)

typedef struct {
    double x;
    double y;
} Point;

int main() {
    Point points[MAX_POINTS];
    int num_points = 0;
    
    printf("Enter number of points (2-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (num_points < 2 || num_points > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between 2 and %d\n", MAX_POINTS);
        return 1;
    }
    
    for (int i = 0; i < num_points; i++) {
        printf("Enter coordinates for point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinates\n");
            return 1;
        }
        
        if (!IS_VALID_COORDINATE(points[i].x) || !IS_VALID_COORDINATE(points[i].y)) {
            fprintf(stderr, "Error: Coordinates must be between -1000 and 1000\n");
            return 1;
        }
    }
    
    double min_distance = __DBL_MAX__;
    int point1 = -1, point2 = -1;
    
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = DISTANCE(points[i].x, points[i].y, points[j].x, points[j].y);
            if (dist < min_distance) {
                min_distance = dist;
                point1 = i;
                point2 = j;
            }
        }
    }
    
    printf("Closest points: Point %d (%.2f, %.2f) and Point %d (%.2f, %.2f)\n",
           point1 + 1, points[point1].x, points[point1].y,
           point2 + 1, points[point2].x, points[point2].y);
    printf("Minimum distance: %.4f\n", min_distance);
    
    double total_distance = 0.0;
    int valid_pairs = 0;
    
    for (int i = 0; i < num_points - 1; i++) {
        for (int j = i + 1; j < num_points; j++) {
            double dist = DISTANCE(points[i].x, points[i].y, points[j].x, points[j].y);
            total_distance += dist;
            valid_pairs++;
        }
    }
    
    if (valid_pairs > 0) {
        double average_distance = total_distance / valid_pairs;
        printf("Average distance between all point pairs: %.4f\n", average_distance);
    }
    
    double bounding_min_x = __DBL_MAX__;
    double bounding_max_x = -__DBL_MAX__;
    double bounding_min_y = __DBL_MAX__;
    double bounding_max_y = -__DBL_MAX__;
    
    for (int i = 0; i < num_points; i++) {
        if (points[i].x < bounding_min_x) bounding_min_x = points[i].x;
        if (points[i].x > bounding_max_x) bounding_max_x = points[i].x;
        if (points[i].y < bounding_min_y) bounding_min_y = points[i].y;
        if (points[i].y > bounding_max_y) bounding_max_y = points[i].y;
    }
    
    printf("Bounding box: (%.2f, %.2f) to (%.2f, %.2f)\n",
           bounding_min_x, bounding_min_y, bounding_max_x, bounding_max_y);
    
    return 0;
}