//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
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

int read_points_from_user(struct PointArray *arr) {
    size_t count;
    printf("Enter number of points (1-%zu): ", (size_t)MAX_POINTS);
    if (scanf("%zu", &count) != 1) {
        return 0;
    }
    if (count < 1 || count > MAX_POINTS) {
        return 0;
    }

    arr->points = malloc(count * sizeof(struct Point));
    if (arr->points == NULL) {
        return 0;
    }
    arr->count = count;

    for (size_t i = 0; i < count; i++) {
        printf("Enter point %zu (x y): ", i + 1);
        if (scanf("%lf %lf", &arr->points[i].x, &arr->points[i].y) != 2) {
            free(arr->points);
            return 0;
        }
    }
    return 1;
}

void compute_distances(struct PointArray *arr, double *distances) {
    for (size_t i = 0; i < arr->count - 1; i++) {
        distances[i] = calculate_distance(arr->points[i], arr->points[i + 1]);
    }
}

double find_min_distance(double *distances, size_t n) {
    double min = distances[0];
    for (size_t i = 1; i < n; i++) {
        if (distances[i] < min) {
            min = distances[i];
        }
    }
    return min;
}

double find_max_distance(double *distances, size_t n) {
    double max = distances[0];
    for (size_t i = 1; i < n; i++) {
        if (distances[i] > max) {
            max = distances[i];
        }
    }
    return max;
}

double calculate_average_distance(double *distances, size_t n) {
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += distances[i];
    }
    return sum / n;
}

int main(void) {
    struct PointArray point_array;
    double distances[MAX_POINTS - 1];
    
    if (!read_points_from_user(&point_array)) {
        printf("Error: Invalid input or memory allocation failed.\n");
        return 1;
    }

    if (point_array.count < 2) {
        printf("At least 2 points are required for distance calculations.\n");
        free(point_array.points);
        return 1;
    }

    compute_distances(&point_array, distances);
    size_t num_distances = point_array.count - 1;

    double min_dist = find_min_distance(distances, num_distances);
    double max_dist = find_max_distance(distances, num_distances);
    double avg_dist = calculate_average_distance(distances, num_distances);

    printf("Distance statistics for %zu points:\n", point_array.count);
    printf("Minimum distance between consecutive points: %.6f\n", min_dist);
    printf("Maximum distance between consecutive points: %.6f\n", max_dist);
    printf("Average distance between consecutive points: %.6f\n", avg_dist);

    free(point_array.points);
    return 0;
}