//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100

struct Point {
    double x;
    double y;
};

int validate_input_count(int count) {
    if (count <= 0 || count > MAX_POINTS) {
        return 0;
    }
    return 1;
}

int validate_coordinate(double coord) {
    if (coord < -1e6 || coord > 1e6) {
        return 0;
    }
    if (coord != coord) {
        return 0;
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    if (dx > 1e150 || dy > 1e150) {
        return -1.0;
    }
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    double total = 0.0;
    for (int i = 1; i < count; i++) {
        double dist = calculate_distance(points[i-1], points[i]);
        if (dist < 0) {
            return -1.0;
        }
        total += dist;
    }
    return total;
}

int read_points(struct Point points[], int max_count) {
    int count;
    printf("Enter number of points (2-%d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    if (!validate_input_count(count)) {
        return -1;
    }
    printf("Enter points as x y coordinates:\n");
    for (int i = 0; i < count; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return -1;
        }
        if (!validate_coordinate(points[i].x) || !validate_coordinate(points[i].y)) {
            return -1;
        }
    }
    return count;
}

int main(void) {
    struct Point points[MAX_POINTS];
    int count = read_points(points, MAX_POINTS);
    if (count < 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    double total_distance = calculate_total_distance(points, count);
    if (total_distance < 0) {
        printf("Error: Calculation overflow\n");
        return 1;
    }
    printf("Total distance along path: %.6f\n", total_distance);
    return 0;
}