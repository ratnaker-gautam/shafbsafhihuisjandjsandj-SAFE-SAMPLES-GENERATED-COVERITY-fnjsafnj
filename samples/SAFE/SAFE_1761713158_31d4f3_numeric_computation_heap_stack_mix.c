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

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    if (sscanf(buffer, "%lf", value) != 1) return 0;
    return 1;
}

int main(void) {
    struct PointArray path;
    if (!init_point_array(&path, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter points for distance calculation (enter 'q' to finish):\n");
    
    while (1) {
        double x, y;
        if (!read_double("Enter x coordinate: ", &x)) break;
        if (!read_double("Enter y coordinate: ", &y)) break;
        
        if (!add_point(&path, x, y)) {
            fprintf(stderr, "Failed to add point\n");
            free_point_array(&path);
            return 1;
        }
        
        if (path.count >= MAX_POINTS) {
            printf("Maximum number of points reached\n");
            break;
        }
        
        char response[10];
        printf("Add another point? (y/n): ");
        if (fgets(response, sizeof(response), stdin) == NULL) break;
        if (response[0] == 'n' || response[0] == 'N') break;
    }

    if (path.count < 2) {
        printf("At least 2 points are required for distance calculation\n");
    } else {
        double total_distance = calculate_total_distance(&path);
        printf("Total distance along the path: %.6f\n", total_distance);
    }

    free_point_array(&path);
    return 0;
}