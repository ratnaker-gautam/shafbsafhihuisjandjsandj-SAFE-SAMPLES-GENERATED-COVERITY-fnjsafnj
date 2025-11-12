//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_POINTS 1000

struct Point {
    double x;
    double y;
};

int read_points(struct Point points[], int max_count) {
    char buffer[256];
    int count = 0;
    
    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    
    while (count < max_count) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (strncmp(buffer, "done", 4) == 0) {
            break;
        }
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            if (!isfinite(x) || !isfinite(y)) {
                printf("Invalid input: non-finite numbers\n");
                continue;
            }
            points[count].x = x;
            points[count].y = y;
            count++;
        } else {
            printf("Invalid format. Please enter 'x y' or 'done'\n");
        }
    }
    
    return count;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    
    if (!isfinite(dx) || !isfinite(dy)) {
        return 0.0;
    }
    
    double distance_squared = dx * dx + dy * dy;
    
    if (!isfinite(distance_squared) || distance_squared < 0) {
        return 0.0;
    }
    
    return sqrt(distance_squared);
}

void find_closest_points(struct Point points[], int count) {
    if (count < 2) {
        printf("Need at least 2 points to find closest pair\n");
        return;
    }
    
    double min_distance = -1.0;
    int point1_idx = -1;
    int point2_idx = -1;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            
            if (!isfinite(dist)) {
                continue;
            }
            
            if (min_distance < 0 || dist < min_distance) {
                min_distance = dist;
                point1_idx = i;
                point2_idx = j;
            }
        }
    }
    
    if (point1_idx >= 0 && point2_idx >= 0 && isfinite(min_distance)) {
        printf("Closest points: (%.2f, %.2f) and (%.2f, %.2f)\n",
               points[point1_idx].x, points[point1_idx].y,
               points[point2_idx].x, points[point2_idx].y);
        printf("Distance: %.6f\n", min_distance);
    } else {
        printf("Could not determine closest points\n");
    }
}

void calculate_centroid(struct Point points[], int count) {
    if (count == 0) {
        printf("No points to calculate centroid\n");
        return;
    }
    
    double sum_x = 0.0;
    double sum_y = 0.0;
    int valid_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (isfinite(points[i].x) && isfinite(points[i].y)) {
            sum_x += points[i].x;
            sum_y += points[i].y;
            valid_count++;
        }
    }
    
    if (valid_count > 0 && isfinite(sum_x) && isfinite(sum_y)) {
        double centroid_x = sum_x / valid_count;
        double centroid_y = sum_y / valid_count;
        
        if (isfinite(centroid_x) && isfinite(centroid_y)) {
            printf("Centroid: (%.6f, %.6f)\n", centroid_x, centroid_y);
        } else {
            printf("Could not calculate valid centroid\n");
        }
    } else {
        printf("No valid points for centroid calculation\n");
    }
}

int main() {
    struct Point points[MAX_POINTS];
    
    int point_count = read_points(points, MAX_POINTS);
    
    if (point_count == 0) {
        printf("No points entered\n");
        return 1;
    }
    
    printf("\nProcessing %d points...\n", point_count);
    
    find_closest_points(points, point_count);
    calculate_centroid(points, point_count);
    
    return 0;
}