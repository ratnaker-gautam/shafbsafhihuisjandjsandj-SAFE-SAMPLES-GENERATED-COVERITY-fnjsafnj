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
    point_array.points = malloc(MAX_POINTS * sizeof(struct Point));
    if (point_array.points == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    point_array.count = 0;

    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    
    char buffer[256];
    while (point_array.count < MAX_POINTS) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (strncmp(buffer, "done", 4) == 0 && (buffer[4] == '\n' || buffer[4] == '\0')) {
            break;
        }
        
        double x, y;
        char temp_buffer[256];
        strcpy(temp_buffer, buffer);
        
        char *token = strtok(temp_buffer, " ");
        if (token == NULL || !validate_input(token, &x)) {
            printf("Invalid x coordinate. Try again.\n");
            continue;
        }
        
        token = strtok(NULL, " \n");
        if (token == NULL || !validate_input(token, &y)) {
            printf("Invalid y coordinate. Try again.\n");
            continue;
        }
        
        if (strtok(NULL, " \n") != NULL) {
            printf("Too many values. Enter exactly two numbers.\n");
            continue;
        }
        
        point_array.points[point_array.count].x = x;
        point_array.points[point_array.count].y = y;
        point_array.count++;
    }

    if (point_array.count < 2) {
        printf("Need at least 2 points to calculate distances.\n");
        free(point_array.points);
        return 1;
    }

    printf("\nCalculating distances between consecutive points:\n");
    struct Point *current = point_array.points;
    struct Point *end = point_array.points + point_array.count - 1;
    
    while (current < end) {
        double distance = calculate_distance(*current, *(current + 1));
        printf("Distance between (%.2f, %.2f) and (%.2f, %.2f): %.2f\n",
               current->x, current->y, (current + 1)->x, (current + 1)->y, distance);
        current++;
    }

    double total_distance = 0.0;
    for (size_t i = 0; i < point_array.count - 1; i++) {
        total_distance += calculate_distance(point_array.points[i], point_array.points[i + 1]);
    }
    
    printf("Total path distance: %.2f\n", total_distance);

    free(point_array.points);
    return 0;
}