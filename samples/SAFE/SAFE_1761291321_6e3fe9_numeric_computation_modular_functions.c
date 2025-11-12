//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100

struct Point {
    double x;
    double y;
};

int read_points(struct Point points[], int max_count) {
    int count;
    printf("Enter number of points (1-%d): ", max_count);
    if (scanf("%d", &count) != 1) {
        return -1;
    }
    if (count < 1 || count > max_count) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        printf("Enter point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            return -1;
        }
    }
    return count;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double total = 0.0;
    for (int i = 0; i < count - 1; i++) {
        total += calculate_distance(points[i], points[i + 1]);
    }
    return total;
}

void find_extreme_points(struct Point points[], int count, struct Point *min, struct Point *max) {
    if (count == 0) {
        return;
    }
    
    *min = points[0];
    *max = points[0];
    
    for (int i = 1; i < count; i++) {
        if (points[i].x < min->x || (points[i].x == min->x && points[i].y < min->y)) {
            *min = points[i];
        }
        if (points[i].x > max->x || (points[i].x == max->x && points[i].y > max->y)) {
            *max = points[i];
        }
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    struct Point min_point, max_point;
    
    int point_count = read_points(points, MAX_POINTS);
    if (point_count <= 0) {
        printf("Invalid input or no points entered.\n");
        return 1;
    }
    
    double total_dist = calculate_total_distance(points, point_count);
    find_extreme_points(points, point_count, &min_point, &max_point);
    
    printf("\nResults:\n");
    printf("Number of points: %d\n", point_count);
    printf("Total distance along path: %.3f\n", total_dist);
    printf("Leftmost/bottom point: (%.3f, %.3f)\n", min_point.x, min_point.y);
    printf("Rightmost/top point: (%.3f, %.3f)\n", max_point.x, max_point.y);
    
    if (point_count >= 2) {
        double first_last_dist = calculate_distance(points[0], points[point_count - 1]);
        printf("Distance between first and last point: %.3f\n", first_last_dist);
    }
    
    return 0;
}