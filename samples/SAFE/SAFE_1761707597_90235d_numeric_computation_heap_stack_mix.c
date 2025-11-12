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
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

int main(void) {
    struct PointArray heap_points;
    struct Point stack_points[MAX_POINTS];
    size_t stack_count = 0;
    char buffer[256];
    double temp_x, temp_y;

    heap_points.points = malloc(MAX_POINTS * sizeof(struct Point));
    if (heap_points.points == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    heap_points.count = 0;

    printf("Enter points (x y), one per line. Empty line to finish:\n");

    while (stack_count < MAX_POINTS && heap_points.count < MAX_POINTS) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        if (buffer[0] == '\n') {
            break;
        }

        if (!validate_input(buffer, &temp_x)) {
            printf("Invalid input. Enter x coordinate: ");
            continue;
        }

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        if (!validate_input(buffer, &temp_y)) {
            printf("Invalid input. Enter y coordinate: ");
            continue;
        }

        if (stack_count < MAX_POINTS / 2) {
            stack_points[stack_count].x = temp_x;
            stack_points[stack_count].y = temp_y;
            stack_count++;
        } else {
            heap_points.points[heap_points.count].x = temp_x;
            heap_points.points[heap_points.count].y = temp_y;
            heap_points.count++;
        }
    }

    if (stack_count == 0 && heap_points.count == 0) {
        printf("No points entered.\n");
        free(heap_points.points);
        return 0;
    }

    double total_distance = 0.0;
    size_t pair_count = 0;

    for (size_t i = 0; i < stack_count - 1; i++) {
        for (size_t j = i + 1; j < stack_count; j++) {
            double dist = calculate_distance(stack_points[i], stack_points[j]);
            total_distance += dist;
            pair_count++;
        }
    }

    for (size_t i = 0; i < heap_points.count - 1; i++) {
        for (size_t j = i + 1; j < heap_points.count; j++) {
            double dist = calculate_distance(heap_points.points[i], heap_points.points[j]);
            total_distance += dist;
            pair_count++;
        }
    }

    for (size_t i = 0; i < stack_count; i++) {
        for (size_t j = 0; j < heap_points.count; j++) {
            double dist = calculate_distance(stack_points[i], heap_points.points[j]);
            total_distance += dist;
            pair_count++;
        }
    }

    if (pair_count > 0) {
        double average_distance = total_distance / pair_count;
        printf("Average distance between %zu point pairs: %.6f\n", pair_count, average_distance);
    } else {
        printf("Not enough points to calculate distances.\n");
    }

    free(heap_points.points);
    return 0;
}