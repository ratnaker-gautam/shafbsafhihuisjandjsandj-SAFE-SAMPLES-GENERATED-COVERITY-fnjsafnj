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
        if (new_capacity > MAX_POINTS) new_capacity = MAX_POINTS;
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

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct PointArray *arr) {
    if (arr->count < 2) return 0.0;
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int main() {
    struct PointArray heap_array;
    if (!init_point_array(&heap_array, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter points (x y), one per line. Enter 'q' to finish.\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'q' || buffer[0] == 'Q') break;
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(&heap_array, x, y)) {
                fprintf(stderr, "Failed to add point, maximum capacity reached\n");
                break;
            }
        } else {
            fprintf(stderr, "Invalid input. Please enter two numbers separated by space.\n");
        }
    }

    if (heap_array.count > 0) {
        struct Point stack_points[10];
        size_t stack_count = heap_array.count < 10 ? heap_array.count : 10;
        for (size_t i = 0; i < stack_count; i++) {
            stack_points[i] = heap_array.points[i];
        }

        double heap_distance = calculate_total_distance(&heap_array);
        printf("Total distance through all %zu points: %.6f\n", heap_array.count, heap_distance);

        if (stack_count >= 2) {
            double stack_distance = 0.0;
            for (size_t i = 1; i < stack_count; i++) {
                stack_distance += calculate_distance(stack_points[i-1], stack_points[i]);
            }
            printf("Distance through first %zu points: %.6f\n", stack_count, stack_distance);
        }
    } else {
        printf("No points entered.\n");
    }

    free_point_array(&heap_array);
    return 0;
}