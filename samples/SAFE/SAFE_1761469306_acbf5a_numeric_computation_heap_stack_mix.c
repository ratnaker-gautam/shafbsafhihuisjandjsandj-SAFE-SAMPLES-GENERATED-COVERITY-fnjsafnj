//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000

struct Point {
    double x;
    double y;
};

struct PointArray {
    struct Point *points;
    size_t count;
};

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *array) {
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        return 0;
    }
    if (num_points < 1 || num_points > MAX_POINTS) {
        return 0;
    }

    array->points = malloc(num_points * sizeof(struct Point));
    if (array->points == NULL) {
        return 0;
    }
    array->count = num_points;

    printf("Enter %d points as x y coordinates:\n", num_points);
    for (size_t i = 0; i < array->count; i++) {
        if (scanf("%lf %lf", &array->points[i].x, &array->points[i].y) != 2) {
            free(array->points);
            return 0;
        }
    }
    return 1;
}

void calculate_distances(struct PointArray *array) {
    if (array->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }

    double distances[MAX_POINTS - 1];
    double total_distance = 0.0;
    double min_distance = -1.0;
    double max_distance = 0.0;

    for (size_t i = 0; i < array->count - 1; i++) {
        double dist = calculate_distance(array->points[i], array->points[i + 1]);
        distances[i] = dist;
        total_distance += dist;

        if (min_distance < 0 || dist < min_distance) {
            min_distance = dist;
        }
        if (dist > max_distance) {
            max_distance = dist;
        }
    }

    printf("\nDistance Analysis:\n");
    printf("Total path distance: %.6f\n", total_distance);
    printf("Average distance between points: %.6f\n", total_distance / (array->count - 1));
    printf("Minimum distance: %.6f\n", min_distance);
    printf("Maximum distance: %.6f\n", max_distance);

    printf("\nIndividual distances:\n");
    for (size_t i = 0; i < array->count - 1; i++) {
        printf("Point %zu to %zu: %.6f\n", i + 1, i + 2, distances[i]);
    }
}

int main(void) {
    struct PointArray point_array;
    point_array.points = NULL;
    point_array.count = 0;

    printf("Point Distance Calculator\n");
    printf("=========================\n");

    if (!read_points_from_user(&point_array)) {
        printf("Error: Invalid input or memory allocation failed.\n");
        return 1;
    }

    calculate_distances(&point_array);

    if (point_array.points != NULL) {
        free(point_array.points);
    }

    return 0;
}