//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000

struct Point {
    double x;
    double y;
};

struct Cluster {
    double center_x;
    double center_y;
    struct Point *points;
    int point_count;
};

double calculate_distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

int main(void) {
    int num_points, num_clusters;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1 || num_points < 1 || num_points > MAX_POINTS) {
        fprintf(stderr, "Invalid number of points\n");
        return 1;
    }
    
    printf("Enter number of clusters (1-10): ");
    if (scanf("%d", &num_clusters) != 1 || num_clusters < 1 || num_clusters > 10) {
        fprintf(stderr, "Invalid number of clusters\n");
        return 1;
    }
    
    struct Point *points = malloc(num_points * sizeof(struct Point));
    if (points == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d points (x y):\n", num_points);
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Invalid point data\n");
            free(points);
            return 1;
        }
    }
    
    struct Cluster clusters[num_clusters];
    for (int i = 0; i < num_clusters; i++) {
        clusters[i].center_x = points[i].x;
        clusters[i].center_y = points[i].y;
        clusters[i].points = malloc(num_points * sizeof(struct Point));
        if (clusters[i].points == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(clusters[j].points);
            }
            free(points);
            return 1;
        }
        clusters[i].point_count = 0;
    }
    
    int max_iterations = 100;
    int changed;
    
    for (int iter = 0; iter < max_iterations; iter++) {
        changed = 0;
        
        for (int i = 0; i < num_clusters; i++) {
            clusters[i].point_count = 0;
        }
        
        for (int i = 0; i < num_points; i++) {
            int best_cluster = 0;
            double best_distance = calculate_distance(points[i].x, points[i].y, 
                                                    clusters[0].center_x, clusters[0].center_y);
            
            for (int j = 1; j < num_clusters; j++) {
                double dist = calculate_distance(points[i].x, points[i].y,
                                               clusters[j].center_x, clusters[j].center_y);
                if (dist < best_distance) {
                    best_distance = dist;
                    best_cluster = j;
                }
            }
            
            if (clusters[best_cluster].point_count < num_points) {
                clusters[best_cluster].points[clusters[best_cluster].point_count] = points[i];
                clusters[best_cluster].point_count++;
            }
        }
        
        for (int i = 0; i < num_clusters; i++) {
            if (clusters[i].point_count == 0) {
                continue;
            }
            
            double sum_x = 0.0;
            double sum_y = 0.0;
            
            for (int j = 0; j < clusters[i].point_count; j++) {
                sum_x += clusters[i].points[j].x;
                sum_y += clusters[i].points[j].y;
            }
            
            double new_center_x = sum_x / clusters[i].point_count;
            double new_center_y = sum_y / clusters[i].point_count;
            
            if (fabs(new_center_x - clusters[i].center_x) > 0.001 ||
                fabs(new_center_y - clusters[i].center_y) > 0.001) {
                changed = 1;
            }
            
            clusters[i].center_x = new_center_x;
            clusters[i].center_y = new_center_y;
        }
        
        if (!changed) {
            break;
        }
    }
    
    printf("\nFinal cluster centers:\n");
    for (int i = 0; i < num_clusters; i++) {
        printf("Cluster %d: (%.3f, %.3f) with %d points\n", 
               i + 1, clusters[i].center_x, clusters[i].center_y, clusters[i].point_count);
    }
    
    for (int i = 0; i < num_clusters; i++) {
        free(clusters[i].points);
    }
    free(points);
    
    return 0;
}