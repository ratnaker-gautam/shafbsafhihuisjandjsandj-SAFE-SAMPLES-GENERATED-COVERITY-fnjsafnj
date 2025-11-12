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

void find_extremes(struct Point points[], int count, double *min_dist, double *max_dist) {
    if (count < 2) {
        *min_dist = 0.0;
        *max_dist = 0.0;
        return;
    }
    
    *min_dist = calculate_distance(points[0], points[1]);
    *max_dist = *min_dist;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            if (dist < *min_dist) {
                *min_dist = dist;
            }
            if (dist > *max_dist) {
                *max_dist = dist;
            }
        }
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    
    int count = read_points(points, MAX_POINTS);
    if (count <= 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("\nPoint analysis for %d points:\n", count);
    
    for (int i = 0; i < count; i++) {
        printf("Point %d: (%.2f, %.2f)\n", i + 1, points[i].x, points[i].y);
    }
    
    if (count >= 2) {
        double total_dist = calculate_total_distance(points, count);
        printf("\nTotal distance along sequence: %.4f\n", total_dist);
        
        double min_dist, max_dist;
        find_extremes(points, count, &min_dist, &max_dist);
        printf("Minimum distance between any two points: %.4f\n", min_dist);
        printf("Maximum distance between any two points: %.4f\n", max_dist);
        
        double avg_dist = total_dist / (count - 1);
        printf("Average distance between consecutive points: %.4f\n", avg_dist);
    } else {
        printf("\nNeed at least 2 points for distance calculations\n");
    }
    
    return 0;
}