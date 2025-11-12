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
    if (initial_capacity == 0 || initial_capacity > MAX_POINTS) {
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
        if (new_capacity > MAX_POINTS) {
            new_capacity = MAX_POINTS;
        }
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

double find_min_distance(struct PointArray *arr) {
    if (arr->count < 2) {
        return -1.0;
    }
    double min_dist = -1.0;
    for (size_t i = 0; i < arr->count - 1; i++) {
        for (size_t j = i + 1; j < arr->count; j++) {
            double dist = calculate_distance(arr->points[i], arr->points[j]);
            if (min_dist < 0 || dist < min_dist) {
                min_dist = dist;
            }
        }
    }
    return min_dist;
}

int main() {
    struct PointArray point_array;
    if (!init_point_array(&point_array, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter points (x y). Enter 'q' to finish.\n");
    char buffer[256];
    while (point_array.count < MAX_POINTS) {
        printf("Point %zu: ", point_array.count + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        if (strlen(buffer) > 0 && buffer[0] == 'q') {
            break;
        }
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(&point_array, x, y)) {
                fprintf(stderr, "Failed to add point\n");
                break;
            }
        } else {
            printf("Invalid input. Please enter two numbers or 'q' to finish.\n");
        }
    }

    if (point_array.count < 2) {
        printf("At least two points are required.\n");
        free_point_array(&point_array);
        return 1;
    }

    double min_distance = find_min_distance(&point_array);
    if (min_distance >= 0) {
        printf("Minimum distance between any two points: %.6f\n", min_distance);
    } else {
        printf("Error calculating minimum distance\n");
    }

    free_point_array(&point_array);
    return 0;
}