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
};

int validate_input(const char *input, double *result) {
    char *endptr;
    *result = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

int main(void) {
    struct PointArray point_array;
    char buffer[256];
    double x, y;
    size_t i, j;
    double total_distance = 0.0;
    int valid_points = 0;

    point_array.points = malloc(MAX_POINTS * sizeof(struct Point));
    if (point_array.points == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    point_array.count = 0;

    printf("Enter points (x y), one per line. Empty line to finish:\n");
    
    while (point_array.count < MAX_POINTS) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (buffer[0] == '\n') {
            break;
        }
        
        if (sscanf(buffer, "%lf %lf", &x, &y) != 2) {
            printf("Invalid input. Please enter two numbers separated by space.\n");
            continue;
        }
        
        if (!isfinite(x) || !isfinite(y)) {
            printf("Invalid numeric values.\n");
            continue;
        }
        
        point_array.points[point_array.count].x = x;
        point_array.points[point_array.count].y = y;
        point_array.count++;
        valid_points++;
    }

    if (valid_points < 2) {
        printf("At least 2 points are required.\n");
        free(point_array.points);
        return 1;
    }

    struct Point centroid;
    centroid.x = 0.0;
    centroid.y = 0.0;
    
    for (i = 0; i < point_array.count; i++) {
        centroid.x += point_array.points[i].x;
        centroid.y += point_array.points[i].y;
    }
    centroid.x /= point_array.count;
    centroid.y /= point_array.count;

    for (i = 0; i < point_array.count; i++) {
        double dist = calculate_distance(point_array.points[i], centroid);
        total_distance += dist;
    }

    double mean_distance = total_distance / point_array.count;

    printf("Centroid: (%.3f, %.3f)\n", centroid.x, centroid.y);
    printf("Mean distance to centroid: %.3f\n", mean_distance);

    struct Point distances[point_array.count];
    for (i = 0; i < point_array.count; i++) {
        distances[i].x = (double)i;
        distances[i].y = calculate_distance(point_array.points[i], centroid);
    }

    for (i = 0; i < point_array.count - 1; i++) {
        for (j = 0; j < point_array.count - i - 1; j++) {
            if (distances[j].y > distances[j + 1].y) {
                struct Point temp = distances[j];
                distances[j] = distances[j + 1];
                distances[j + 1] = temp;
            }
        }
    }

    printf("Points sorted by distance to centroid:\n");
    for (i = 0; i < point_array.count; i++) {
        int idx = (int)distances[i].x;
        printf("Point %d: (%.3f, %.3f) - Distance: %.3f\n", 
               idx, point_array.points[idx].x, point_array.points[idx].y, distances[i].y);
    }

    free(point_array.points);
    return 0;
}