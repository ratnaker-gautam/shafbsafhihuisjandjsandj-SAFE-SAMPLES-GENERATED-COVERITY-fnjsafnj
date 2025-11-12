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
    if (arr == NULL || initial_capacity == 0) return 0;
    arr->points = malloc(initial_capacity * sizeof(struct Point));
    if (arr->points == NULL) return 0;
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 1;
}

int add_point(struct PointArray *arr, double x, double y) {
    if (arr == NULL) return 0;
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

void free_point_array(struct PointArray *arr) {
    if (arr != NULL) {
        free(arr->points);
        arr->points = NULL;
        arr->count = 0;
        arr->capacity = 0;
    }
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_user(struct PointArray *arr) {
    if (arr == NULL) return 0;
    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strncmp(buffer, "done", 4) == 0) break;
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(arr, x, y)) {
                printf("Failed to add point. Maximum capacity reached.\n");
                return 0;
            }
        } else {
            printf("Invalid input. Please enter two numbers or 'done'.\n");
        }
    }
    return 1;
}

void calculate_distances(struct PointArray *arr) {
    if (arr == NULL || arr->count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        return;
    }
    double distances[MAX_POINTS];
    size_t dist_count = 0;
    for (size_t i = 0; i < arr->count - 1 && dist_count < MAX_POINTS; i++) {
        for (size_t j = i + 1; j < arr->count && dist_count < MAX_POINTS; j++) {
            distances[dist_count] = calculate_distance(arr->points[i], arr->points[j]);
            dist_count++;
        }
    }
    printf("Pairwise distances:\n");
    for (size_t i = 0; i < dist_count; i++) {
        printf("%.2f\n", distances[i]);
    }
    double min_dist = distances[0];
    double max_dist = distances[0];
    double sum = distances[0];
    for (size_t i = 1; i < dist_count; i++) {
        if (distances[i] < min_dist) min_dist = distances[i];
        if (distances[i] > max_dist) max_dist = distances[i];
        sum += distances[i];
    }
    printf("Minimum distance: %.2f\n", min_dist);
    printf("Maximum distance: %.2f\n", max_dist);
    printf("Average distance: %.2f\n", sum / dist_count);
}

int main() {
    struct PointArray arr;
    if (!init_point_array(&arr, 10)) {
        printf("Failed to initialize point array.\n");
        return 1;
    }
    if (!read_points_from_user(&arr)) {
        free_point_array(&arr);
        return 1;
    }
    if (arr.count == 0) {
        printf("No points entered.\n");
        free_point_array(&arr);
        return 1;
    }
    calculate_distances(&arr);
    free_point_array(&arr);
    return 0;
}