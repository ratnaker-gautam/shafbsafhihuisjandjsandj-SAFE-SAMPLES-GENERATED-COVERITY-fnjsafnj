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

int read_points(struct Point* points, int count) {
    for (int i = 0; i < count; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return 0;
        }
    }
    return 1;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct Point* points, int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        total += calculate_distance(points[i], points[i + 1]);
    }
    return total;
}

void print_results(struct Point* points, int count, double total_distance) {
    printf("Points entered:\n");
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    printf("Total distance between consecutive points: %.4f\n", total_distance);
}

int main(void) {
    struct Point points[MAX_POINTS];
    int point_count;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        fprintf(stderr, "Error: Invalid input for point count\n");
        return 1;
    }
    
    if (!validate_input_count(point_count)) {
        fprintf(stderr, "Error: Point count must be between 1 and %d\n", MAX_POINTS);
        return 1;
    }
    
    printf("Enter %d points as 'x y' coordinates:\n", point_count);
    if (!read_points(points, point_count)) {
        fprintf(stderr, "Error: Invalid point coordinates\n");
        return 1;
    }
    
    double total_distance = calculate_total_distance(points, point_count);
    print_results(points, point_count, total_distance);
    
    return 0;
}