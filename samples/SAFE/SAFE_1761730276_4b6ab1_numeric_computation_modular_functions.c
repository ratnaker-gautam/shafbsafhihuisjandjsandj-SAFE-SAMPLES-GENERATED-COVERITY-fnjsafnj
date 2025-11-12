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

double find_min_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double min_dist = -1.0;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (min_dist < 0 || dist < min_dist) {
                min_dist = dist;
            }
        }
    }
    return min_dist;
}

double find_max_distance(struct Point points[], int count) {
    if (count < 2) {
        return 0.0;
    }
    
    double max_dist = 0.0;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (dist > max_dist) {
                max_dist = dist;
            }
        }
    }
    return max_dist;
}

double calculate_centroid_x(struct Point points[], int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += points[i].x;
    }
    return sum / count;
}

double calculate_centroid_y(struct Point points[], int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += points[i].y;
    }
    return sum / count;
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Invalid input or too many points\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Number of points: %d\n", count);
    
    if (count >= 2) {
        double min_dist = find_min_distance(points, count);
        double max_dist = find_max_distance(points, count);
        printf("Minimum distance between points: %.3f\n", min_dist);
        printf("Maximum distance between points: %.3f\n", max_dist);
    }
    
    double centroid_x = calculate_centroid_x(points, count);
    double centroid_y = calculate_centroid_y(points, count);
    printf("Centroid of points: (%.3f, %.3f)\n", centroid_x, centroid_y);
    
    printf("\nPoint distances from centroid:\n");
    for (int i = 0; i < count; i++) {
        struct Point centroid = {centroid_x, centroid_y};
        double dist = calculate_distance(points[i], centroid);
        printf("Point %d: (%.3f, %.3f) - Distance: %.3f\n", 
               i + 1, points[i].x, points[i].y, dist);
    }
    
    return 0;
}