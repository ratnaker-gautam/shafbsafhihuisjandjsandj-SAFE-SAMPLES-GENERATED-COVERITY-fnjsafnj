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
        if (points[i].x < min->x) min->x = points[i].x;
        if (points[i].y < min->y) min->y = points[i].y;
        if (points[i].x > max->x) max->x = points[i].x;
        if (points[i].y > max->y) max->y = points[i].y;
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    double total_dist = calculate_total_distance(points, count);
    printf("Total distance along path: %.3f\n", total_dist);
    
    struct Point min_point, max_point;
    find_extreme_points(points, count, &min_point, &max_point);
    
    printf("Minimum point: (%.3f, %.3f)\n", min_point.x, min_point.y);
    printf("Maximum point: (%.3f, %.3f)\n", max_point.x, max_point.y);
    
    if (count >= 2) {
        double bounding_width = max_point.x - min_point.x;
        double bounding_height = max_point.y - min_point.y;
        double bounding_area = bounding_width * bounding_height;
        printf("Bounding box area: %.3f\n", bounding_area);
    }
    
    return 0;
}