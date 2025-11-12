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
    size_t capacity;
};

int initPointArray(struct PointArray *arr, size_t initial_capacity) {
    if (initial_capacity == 0) return 0;
    arr->points = malloc(initial_capacity * sizeof(struct Point));
    if (arr->points == NULL) return 0;
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

void freePointArray(struct PointArray *arr) {
    free(arr->points);
    arr->points = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

int addPoint(struct PointArray *arr, double x, double y) {
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity <= arr->capacity) return 0;
        struct Point *new_points = realloc(arr->points, new_capacity * sizeof(struct Point));
        if (new_points == NULL) return 0;
        arr->points = new_points;
        arr->capacity = new_capacity;
    }
    arr->points[arr->count].x = x;
    arr->points[arr->count].y = y;
    arr->count++;
    return 1;
}

double calculateDistance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int readPointsFromUser(struct PointArray *arr) {
    printf("Enter number of points (1-%zu): ", MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) return 0;
    if (num_points < 1 || num_points > MAX_POINTS) return 0;
    
    for (int i = 0; i < num_points; i++) {
        double x, y;
        printf("Enter point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &x, &y) != 2) return 0;
        if (!addPoint(arr, x, y)) return 0;
    }
    return 1;
}

void calculateDistances(struct PointArray *arr) {
    if (arr->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }
    
    double distances[MAX_POINTS];
    size_t distance_count = 0;
    
    for (size_t i = 0; i < arr->count - 1; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            if (distance_count >= MAX_POINTS) break;
            distances[distance_count] = calculateDistance(arr->points[i], arr->points[j]);
            distance_count++;
        }
    }
    
    printf("\nCalculated %zu distances:\n", distance_count);
    double min_dist = distances[0];
    double max_dist = distances[0];
    double sum = 0.0;
    
    for (size_t i = 0; i < distance_count; i++) {
        printf("Distance %zu: %.3f\n", i + 1, distances[i]);
        if (distances[i] < min_dist) min_dist = distances[i];
        if (distances[i] > max_dist) max_dist = distances[i];
        sum += distances[i];
    }
    
    printf("\nStatistics:\n");
    printf("Minimum distance: %.3f\n", min_dist);
    printf("Maximum distance: %.3f\n", max_dist);
    printf("Average distance: %.3f\n", sum / distance_count);
}

int main(void) {
    struct PointArray point_array;
    
    if (!initPointArray(&point_array, 10)) {
        fprintf(stderr, "Failed to initialize point array.\n");
        return 1;
    }
    
    printf("Point Distance Calculator\n");
    printf("=========================\n");
    
    if (!readPointsFromUser(&point_array)) {
        fprintf(stderr, "Error reading points from user.\n");
        freePointArray(&point_array);
        return 1;
    }
    
    calculateDistances(&point_array);
    freePointArray(&point_array);
    
    return 0;
}