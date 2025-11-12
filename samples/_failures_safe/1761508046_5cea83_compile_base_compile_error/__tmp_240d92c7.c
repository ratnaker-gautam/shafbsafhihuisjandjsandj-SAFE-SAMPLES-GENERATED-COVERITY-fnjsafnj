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

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
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
    int num_points_stack;
    
    printf("Enter number of points to generate (1-%d): ", MAX_POINTS);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        free_point_array(&points_heap);
        return 1;
    }
    char *endptr;
    num_points_stack = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || num_points_stack <= 0 || num_points_stack > MAX_POINTS) {
        fprintf(stderr, "Invalid number of points\n");
        free_point_array(&points_heap);
        return 1;
    }

    for (int i = 0; i < num_points_stack; i++) {
        double x, y;
        printf("Point %d:\n", i + 1);
        if (!read_double("  Enter x coordinate: ", &x)) {
            fprintf(stderr, "Invalid x coordinate\n");
            free_point_array(&points_heap);
            return 1;
        }
        if (!read_double("  Enter y coordinate: ", &y)) {
            fprintf(stderr, "Invalid y coordinate\n");
            free_point_array(&points_heap);
            return 1;
        }
        if (!add_point(&points_heap, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&points_heap);
            return 1;
        }
    }

    if (points_heap.count == 0) {
        printf("No points to process\n");
        free_point_array(&points_heap);
        return 0;
    }

    double total_distance = 0.0;
    int valid_pairs = 0;
    
    for (size_t i = 0; i < points_heap.count; i++) {
        for (size_t j = i + 1; j < points_heap.count; j++) {
            double dist = calculate_distance(points_heap.points[i], points_heap.points[j]);
            if (!isnan(dist) && !isinf(dist)) {
                total_distance += dist;
                valid_pairs++;
            }
        }
    }

    double avg_distance = 0.0;
    if (valid_pairs > 0) {
        avg_distance = total_distance / valid_pairs;
    }

    double min_dist_to_origin = INFINITY;
    double max_dist_to_origin = 0.0;
    
    for (size_t i = 0; i < points_heap.count; i++) {
        double dist = calculate_distance(points_heap.points[i], origin_stack);
        if (!isnan(dist) && !isinf(dist)) {
            if (dist < min_dist_to_origin) {
                min_dist_to_origin = dist;
            }
            if (dist > max_dist_to_origin) {
                max_dist_to_origin = dist;
            }
        }
    }