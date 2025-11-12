//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer) {
        return 0;
    }
    return 1;
}

int main(void) {
    struct PointArray points_heap;
    if (!init_point_array(&points_heap, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    struct Point origin_stack = {0.0, 0.0};
    double total_distance = 0.0;
    int point_count = 0;

    printf("Enter points (x y). Enter 'q' to finish.\n");
    
    while (point_count < MAX_POINTS) {
        double x, y;
        if (!read_double("Enter x: ", &x)) {
            break;
        }
        if (!read_double("Enter y: ", &y)) {
            break;
        }

        if (!add_point(&points_heap, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&points_heap);
            return 1;
        }

        struct Point current_stack = {x, y};
        double distance = calculate_distance(origin_stack, current_stack);
        total_distance += distance;
        point_count++;

        printf("Point %d: (%.2f, %.2f), Distance from origin: %.2f\n", 
               point_count, x, y, distance);
    }

    if (point_count > 0) {
        double average_distance = total_distance / point_count;
        printf("\nStatistics:\n");
        printf("Total points: %d\n", point_count);
        printf("Average distance from origin: %.2f\n", average_distance);

        if (point_count >= 2) {
            double min_distance = calculate_distance(points_heap.points[0], points_heap.points[1]);
            double max_distance = min_distance;
            
            for (int i = 0; i < point_count; i++) {
                for (int j = i + 1; j < point_count; j++) {
                    double dist = calculate_distance(points_heap.points[i], points_heap.points[j]);
                    if (dist < min_distance) min_distance = dist;
                    if (dist > max_distance) max_distance = dist;
                }
            }
            
            printf("Minimum distance between points: %.2f\n", min_distance);
            printf("Maximum distance between points: %.2f\n", max_distance);
        }
    }

    free_point_array(&points_heap);
    return 0;
}