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

double calculate_total_distance(struct PointArray *arr) {
    if (arr->count < 2) {
        return 0.0;
    }
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int main() {
    struct PointArray path;
    if (!init_point_array(&path, 10)) {
        fprintf(stderr, "Failed to initialize point array\n");
        return 1;
    }

    printf("Enter points (x y), one per line. Enter 'q' to finish.\n");
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'q' && (buffer[1] == '\n' || buffer[1] == '\0')) {
            break;
        }
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!add_point(&path, x, y)) {
                fprintf(stderr, "Failed to add point\n");
                free_point_array(&path);
                return 1;
            }
            if (path.count >= MAX_POINTS) {
                printf("Maximum number of points reached\n");
                break;
            }
        } else {
            printf("Invalid input. Please enter two numbers or 'q' to finish.\n");
        }
    }

    if (path.count > 0) {
        printf("\nPath points:\n");
        for (size_t i = 0; i < path.count; i++) {
            printf("Point %zu: (%.2f, %.2f)\n", i+1, path.points[i].x, path.points[i].y);
        }
        
        double total_dist = calculate_total_distance(&path);
        printf("\nTotal path distance: %.4f\n", total_dist);
        
        if (path.count >= 3) {
            struct Point centroid_stack;
            centroid_stack.x = 0.0;
            centroid_stack.y = 0.0;
            
            for (size_t i = 0; i < path.count; i++) {
                centroid_stack.x += path.points[i].x;
                centroid_stack.y += path.points[i].y;
            }
            centroid_stack.x /= path.count;
            centroid_stack.y /= path.count;
            
            printf("Centroid: (%.4f, %.4f)\n", centroid_stack.x, centroid_stack.y);
        }
    } else {
        printf("No points entered.\n");
    }

    free_point_array(&path);
    return 0;
}