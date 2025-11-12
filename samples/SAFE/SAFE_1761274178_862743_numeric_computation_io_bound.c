//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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
            if (x >= -1e6 && x <= 1e6 && y >= -1e6 && y <= 1e6) {
                points[count].x = x;
                points[count].y = y;
                count++;
            } else {
                printf("Coordinates must be between -1e6 and 1e6\n");
            }
        } else {
            printf("Invalid input. Use format: x y\n");
        }
    }
    
    return count;
}

double calculate_distance(struct Point p1, struct Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    
    if (dx > 1e150 || dy > 1e150 || dx < -1e150 || dy < -1e150) {
        return -1.0;
    }
    
    double distance_squared = dx * dx + dy * dy;
    
    if (distance_squared > 1e300) {
        return -1.0;
    }
    
    return sqrt(distance_squared);
}

void find_closest_points(struct Point points[], int count) {
    if (count < 2) {
        printf("Need at least 2 points to find closest pair\n");
        return;
    }
    
    double min_distance = -1.0;
    int point1 = -1;
    int point2 = -1;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            double dist = calculate_distance(points[i], points[j]);
            
            if (dist >= 0 && (min_distance < 0 || dist < min_distance)) {
                min_distance = dist;
                point1 = i;
                point2 = j;
            }
        }
    }
    
    if (min_distance >= 0) {
        printf("Closest points: (%.2f, %.2f) and (%.2f, %.2f)\n",
               points[point1].x, points[point1].y,
               points[point2].x, points[point2].y);
        printf("Distance: %.6f\n", min_distance);
    } else {
        printf("Could not calculate valid distance\n");
    }
}

double calculate_centroid(struct Point points[], int count) {
    if (count == 0) {
        return 0.0;
    }
    
    double sum_x = 0.0;
    double sum_y = 0.0;
    
    for (int i = 0; i < count; i++) {
        if (sum_x > 1e300 - fabs(points[i].x) || sum_y > 1e300 - fabs(points[i].y)) {
            return -1.0;
        }
        sum_x += points[i].x;
        sum_y += points[i].y;
    }
    
    double centroid_x = sum_x / count;
    double centroid_y = sum_y / count;
    
    printf("Centroid: (%.6f, %.6f)\n", centroid_x, centroid_y);
    
    double total_distance = 0.0;
    int valid_count = 0;
    
    for (int i = 0; i < count; i++) {
        struct Point centroid = {centroid_x, centroid_y};
        double dist = calculate_distance(points[i], centroid);
        if (dist >= 0) {
            total_distance += dist;
            valid_count++;
        }
    }
    
    if (valid_count > 0) {
        return total_distance / valid_count;
    }
    return -1.0;
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
    
    double avg_distance = calculate_centroid(points, point_count);
    if (avg_distance >= 0) {
        printf("Average distance from centroid: %.6f\n", avg_distance);
    } else {
        printf("Could not calculate average distance from centroid\n");
    }
    
    return 0;
}