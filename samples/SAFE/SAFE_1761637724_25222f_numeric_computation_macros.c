//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_POINTS 100
#define MIN_POINTS 2
#define MAX_COORD 1000.0
#define MIN_COORD -1000.0
#define DISTANCE(p1, p2) sqrt(((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)))
#define VALID_COORD(c) ((c) >= MIN_COORD && (c) <= MAX_COORD)

typedef struct {
    double x;
    double y;
} Point;

int main() {
    int n;
    Point points[MAX_POINTS];
    
    printf("Enter number of points (2-100): ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (n < MIN_POINTS || n > MAX_POINTS) {
        fprintf(stderr, "Error: Number of points must be between %d and %d\n", MIN_POINTS, MAX_POINTS);
        return 1;
    }
    
    printf("Enter coordinates for %d points (x y):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Error: Invalid coordinate input\n");
            return 1;
        }
        
        if (!VALID_COORD(points[i].x) || !VALID_COORD(points[i].y)) {
            fprintf(stderr, "Error: Coordinates must be between %.1f and %.1f\n", MIN_COORD, MAX_COORD);
            return 1;
        }
    }
    
    double min_distance = -1.0;
    int point1 = -1, point2 = -1;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = DISTANCE(points[i], points[j]);
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
                point1 = i;
                point2 = j;
            }
        }
    }
    
    printf("Closest points: (%g, %g) and (%g, %g)\n", 
           points[point1].x, points[point1].y, 
           points[point2].x, points[point2].y);
    printf("Distance: %g\n", min_distance);
    
    double total_distance = 0.0;
    int count = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = DISTANCE(points[i], points[j]);
            total_distance += dist;
            count++;
        }
    }
    
    if (count > 0) {
        double avg_distance = total_distance / count;
        printf("Average distance between all points: %g\n", avg_distance);
    }
    
    double centroid_x = 0.0, centroid_y = 0.0;
    for (int i = 0; i < n; i++) {
        centroid_x += points[i].x;
        centroid_y += points[i].y;
    }
    centroid_x /= n;
    centroid_y /= n;
    
    printf("Centroid of all points: (%g, %g)\n", centroid_x, centroid_y);
    
    return 0;
}