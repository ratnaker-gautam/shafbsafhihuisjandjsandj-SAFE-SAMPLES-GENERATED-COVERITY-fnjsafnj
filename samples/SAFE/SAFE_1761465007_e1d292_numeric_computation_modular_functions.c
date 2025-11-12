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
        if (points[i].x < min->x) {
            min->x = points[i].x;
        }
        if (points[i].y < min->y) {
            min->y = points[i].y;
        }
        if (points[i].x > max->x) {
            max->x = points[i].x;
        }
        if (points[i].y > max->y) {
            max->y = points[i].y;
        }
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    struct Point min_point, max_point;
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\nPoints entered:\n");
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    
    double total_dist = calculate_total_distance(points, count);
    printf("\nTotal distance between consecutive points: %.4f\n", total_dist);
    
    find_extreme_points(points, count, &min_point, &max_point);
    printf("Minimum coordinates: (%.2f, %.2f)\n", min_point.x, min_point.y);
    printf("Maximum coordinates: (%.2f, %.2f)\n", max_point.x, max_point.y);
    
    if (count >= 2) {
        double max_segment = 0.0;
        int max_index = 0;
        for (int i = 0; i < count - 1; i++) {
            double dist = calculate_distance(points[i], points[i + 1]);
            if (dist > max_segment) {
                max_segment = dist;
                max_index = i;
            }
        }
        printf("Longest segment: between points %d and %d (distance: %.4f)\n", 
               max_index + 1, max_index + 2, max_segment);
    }
    
    return 0;
}