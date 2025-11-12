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

double calculate_total_path_length(struct PointArray *arr) {
    if (arr->count < 2) {
        return 0.0;
    }
    double total = 0.0;
    for (size_t i = 1; i < arr->count; i++) {
        total += calculate_distance(arr->points[i-1], arr->points[i]);
    }
    return total;
}

int read_double(const char *prompt, double *value) {
    char buffer[256];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buffer, "%lf", value) != 1) {
        return 0;
    }
    return 1;
}

int main(void) {
    struct PointArray path;
    size_t initial_size;
    
    printf("Enter initial number of points (1-%zu): ", (size_t)MAX_POINTS);
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    if (sscanf(buffer, "%zu", &initial_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (initial_size < 1 || initial_size > MAX_POINTS) {
        printf("Invalid size\n");
        return 1;
    }
    
    if (!init_point_array(&path, initial_size)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter points (x y). Enter 'q' to finish.\n");
    
    while (path.count < MAX_POINTS) {
        double x, y;
        printf("Point %zu: ", path.count + 1);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(buffer) > 0 && buffer[0] == 'q') {
            break;
        }
        
        if (sscanf(buffer, "%lf %lf", &x, &y) != 2) {
            printf("Invalid input. Use format: x y\n");
            continue;
        }
        
        if (!add_point(&path, x, y)) {
            printf("Cannot add more points\n");
            break;
        }
    }
    
    if (path.count < 2) {
        printf("At least 2 points required\n");
        free_point_array(&path);
        return 1;
    }
    
    double total_length = calculate_total_path_length(&path);
    printf("Total path length: %.6f\n", total_length);
    
    struct Point centroid;
    centroid.x = 0.0;
    centroid.y = 0.0;
    for (size_t i = 0; i < path.count; i++) {
        centroid.x += path.points[i].x;
        centroid.y += path.points[i].y;
    }
    centroid.x /= path.count;
    centroid.y /= path.count;
    
    printf("Centroid: (%.6f, %.6f)\n", centroid.x, centroid.y);
    
    free_point_array(&path);
    return 0;
}