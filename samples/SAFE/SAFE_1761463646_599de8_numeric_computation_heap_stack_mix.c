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

int init_point_array(struct PointArray *arr, size_t initial_capacity) {
    if (initial_capacity == 0) return 0;
    arr->points = malloc(initial_capacity * sizeof(struct Point));
    if (arr->points == NULL) return 0;
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

void free_point_array(struct PointArray *arr) {
    if (arr->points != NULL) {
        free(arr->points);
        arr->points = NULL;
    }
    arr->count = 0;
    arr->capacity = 0;
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        if (new_capacity < arr->capacity) return 0;
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

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    struct PointArray points_heap;
    if (!init_point_array(&points_heap, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter number of points (1-%d): ", MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Invalid input\n");
        free_point_array(&points_heap);
        return 1;
    }

    if (num_points < 1 || num_points > MAX_POINTS) {
        fprintf(stderr, "Number of points out of range\n");
        free_point_array(&points_heap);
        return 1;
    }

    struct Point origin = {0.0, 0.0};
    double distances_stack[MAX_POINTS];

    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < num_points; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            fprintf(stderr, "Invalid point input\n");
            free_point_array(&points_heap);
            return 1;
        }
        if (!add_point(&points_heap, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&points_heap);
            return 1;
        }
        distances_stack[i] = calculate_distance(points_heap.points[i], origin);
    }

    double min_distance = distances_stack[0];
    double max_distance = distances_stack[0];
    double sum = distances_stack[0];
    int min_index = 0;
    int max_index = 0;

    for (int i = 1; i < num_points; i++) {
        if (distances_stack[i] < min_distance) {
            min_distance = distances_stack[i];
            min_index = i;
        }
        if (distances_stack[i] > max_distance) {
            max_distance = distances_stack[i];
            max_index = i;
        }
        sum += distances_stack[i];
    }

    printf("\nDistance statistics from origin (0,0):\n");
    printf("Closest point: (%.2f, %.2f) distance: %.2f\n", 
           points_heap.points[min_index].x, points_heap.points[min_index].y, min_distance);
    printf("Farthest point: (%.2f, %.2f) distance: %.2f\n", 
           points_heap.points[max_index].x, points_heap.points[max_index].y, max_distance);
    printf("Average distance: %.2f\n", sum / num_points);

    free_point_array(&points_heap);
    return 0;
}