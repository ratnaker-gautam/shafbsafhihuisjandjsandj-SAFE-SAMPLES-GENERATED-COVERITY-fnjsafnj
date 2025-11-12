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
    if (initial_capacity == 0) {
        return 0;
    }
    arr->points = malloc(initial_capacity * sizeof(struct Point));
    if (arr->points == NULL) {
        return 0;
    }
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
        if (new_capacity <= arr->capacity) {
            return 0;
        }
        struct Point *new_points = realloc(arr->points, new_capacity * sizeof(struct Point));
        if (new_points == NULL) {
            return 0;
        }
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
    struct PointArray heap_points;
    if (!init_point_array(&heap_points, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter number of points (1-%d): ", MAX_POINTS);
    int num_points;
    if (scanf("%d", &num_points) != 1) {
        fprintf(stderr, "Invalid input\n");
        free_point_array(&heap_points);
        return 1;
    }

    if (num_points < 1 || num_points > MAX_POINTS) {
        fprintf(stderr, "Number of points must be between 1 and %d\n", MAX_POINTS);
        free_point_array(&heap_points);
        return 1;
    }

    struct Point stack_points[MAX_POINTS];
    size_t stack_count = 0;

    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < num_points; i++) {
        double x, y;
        if (scanf("%lf %lf", &x, &y) != 2) {
            fprintf(stderr, "Invalid point coordinates\n");
            free_point_array(&heap_points);
            return 1;
        }

        if (i % 2 == 0) {
            if (!add_point(&heap_points, x, y)) {
                fprintf(stderr, "Failed to add point to heap array\n");
                free_point_array(&heap_points);
                return 1;
            }
        } else {
            if (stack_count >= MAX_POINTS) {
                fprintf(stderr, "Stack array full\n");
                free_point_array(&heap_points);
                return 1;
            }
            stack_points[stack_count].x = x;
            stack_points[stack_count].y = y;
            stack_count++;
        }
    }

    double total_distance = 0.0;
    size_t pair_count = 0;

    for (size_t i = 0; i < heap_points.count; i++) {
        for (size_t j = 0; j < stack_count; j++) {
            double dist = calculate_distance(heap_points.points[i], stack_points[j]);
            if (!isnan(dist) && !isinf(dist)) {
                total_distance += dist;
                pair_count++;
            }
        }
    }

    for (size_t i = 0; i < heap_points.count - 1; i++) {
        for (size_t j = i + 1; j < heap_points.count; j++) {
            double dist = calculate_distance(heap_points.points[i], heap_points.points[j]);
            if (!isnan(dist) && !isinf(dist)) {
                total_distance += dist;
                pair_count++;
            }
        }
    }

    for (size_t i = 0; i < stack_count - 1; i++) {
        for (size_t j = i + 1; j < stack_count; j++) {
            double dist = calculate_distance(stack_points[i], stack_points[j]);
            if (!isnan(dist) && !isinf(dist)) {
                total_distance += dist;
                pair_count++;
            }
        }
    }

    if (pair_count > 0) {
        double average_distance = total_distance / pair_count;
        printf("Average distance between points: %.6f\n", average_distance);
        printf("Total pairs considered: %zu\n", pair_count);
    } else {
        printf("No valid point pairs to calculate distance\n");
    }

    free_point_array(&heap_points);
    return 0;
}