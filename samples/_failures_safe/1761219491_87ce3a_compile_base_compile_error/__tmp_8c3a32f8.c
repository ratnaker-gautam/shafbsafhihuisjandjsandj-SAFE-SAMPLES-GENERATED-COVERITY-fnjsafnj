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

int main(void) {
    struct PointArray heap_points;
    if (!init_point_array(&heap_points, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    struct Point stack_points[MAX_POINTS];
    size_t stack_count = 0;

    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strncmp(buffer, "done", 4) == 0) {
            break;
        }
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (stack_count < MAX_POINTS) {
                stack_points[stack_count].x = x;
                stack_points[stack_count].y = y;
                stack_count++;
            } else {
                if (!add_point(&heap_points, x, y)) {
                    fprintf(stderr, "Failed to add point to heap array\n");
                    free_point_array(&heap_points);
                    return 1;
                }
            }
        } else {
            fprintf(stderr, "Invalid input. Please enter two numbers or 'done'.\n");
        }
    }

    if (stack_count == 0 && heap_points.count == 0) {
        printf("No points entered.\n");
        free_point_array(&heap_points);
        return 0;
    }

    double total_distance = 0.0;
    size_t total_points = stack_count + heap_points.count;

    for (size_t i = 0; i < stack_count - 1; i++) {
        total_distance += calculate_distance(stack_points[i], stack_points[i + 1]);
    }

    for (size_t i = 0; i < heap_points.count - 1; i++) {
        total_distance += calculate_distance(heap_points.points[i], heap_points.points[i + 1]);
    }

    if (stack_count > 0 && heap_points.count > 0) {
        total_distance += calculate_distance(stack_points[stack_count - 1], heap_points.points[0]);
    }

    printf("Total distance through %zu points: %.6f\n", total_points, total_distance);

    double min_x = (stack_count > 0) ? stack_points[0].x : heap_points.points[0].x;
    double max_x = min_x;
    double min_y = (stack_count > 0) ? stack_points[0].y : heap_points.points[0].y;
    double max_y = min_y;

    for (size_t i = 0; i < stack_count; i++) {
        if (stack_points[i].x < min_x) min_x = stack_points[i].x;
        if (stack_points[i].x > max_x) max_x = stack_points[i].x;
        if (stack_points[i].y < min_y) min_y = stack_points[i].y;
        if (stack_points[i].y > max_y) max_y = stack_points[i].y;
    }

    for (size_t i = 0; i < heap_points.count; i++) {
        if (heap_points.points[i].x < min_x) min_x = heap_points.points[i].x;
        if (heap_points.points[i].x > max_x) max_x = heap_points.points[i].x;
        if (heap_points.points[i].y < min_y) min_y = heap_points.points[i].y;
        if (heap_points.points[i].y > max_y) max_y = heap_points.points[i].y;
    }