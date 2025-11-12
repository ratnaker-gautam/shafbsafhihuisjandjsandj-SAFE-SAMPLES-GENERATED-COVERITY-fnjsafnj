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

void calculate_centroid(struct Point points[], int count, struct Point *centroid) {
    centroid->x = 0.0;
    centroid->y = 0.0;
    
    for (int i = 0; i < count; i++) {
        centroid->x += points[i].x;
        centroid->y += points[i].y;
    }
    
    if (count > 0) {
        centroid->x /= count;
        centroid->y /= count;
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    struct Point centroid;
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\nPoint analysis results:\n");
    printf("Number of points: %d\n", count);
    
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    
    double total_dist = calculate_total_distance(points, count);
    printf("Total distance between consecutive points: %.4f\n", total_dist);
    
    calculate_centroid(points, count, &centroid);
    printf("Centroid: (%.4f, %.4f)\n", centroid.x, centroid.y);
    
    if (count >= 2) {
        double avg_distance = total_dist / (count - 1);
        printf("Average distance between points: %.4f\n", avg_distance);
    }
    
    return 0;
}