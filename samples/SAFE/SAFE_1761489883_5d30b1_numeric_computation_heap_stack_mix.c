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
    double centroid_x;
    double centroid_y;
    struct Point* points;
    int count;
};

double calculate_distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    int num_points, num_clusters;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &num_points) != 1 || num_points < 1 || num_points > MAX_POINTS) {
        printf("Invalid number of points\n");
        return 1;
    }
    
    printf("Enter number of clusters (1-10): ");
    if (scanf("%d", &num_clusters) != 1 || num_clusters < 1 || num_clusters > 10) {
        printf("Invalid number of clusters\n");
        return 1;
    }
    
    struct Point points[MAX_POINTS];
    
    printf("Enter points (x y):\n");
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            printf("Invalid point data\n");
            return 1;
        }
    }
    
    struct Cluster* clusters = malloc(num_clusters * sizeof(struct Cluster));
    if (clusters == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < num_clusters; i++) {
        clusters[i].points = malloc(num_points * sizeof(struct Point));
        if (clusters[i].points == NULL) {
            printf("Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(clusters[j].points);
            }
            free(clusters);
            return 1;
        }
        clusters[i].count = 0;
        clusters[i].centroid_x = points[i % num_points].x;
        clusters[i].centroid_y = points[i % num_points].y;
    }
    
    int max_iterations = 100;
    int changed;
    
    for (int iter = 0; iter < max_iterations; iter++) {
        changed = 0;
        
        for (int i = 0; i < num_clusters; i++) {
            clusters[i].count = 0;
        }
        
        for (int i = 0; i < num_points; i++) {
            int best_cluster = 0;
            double best_distance = calculate_distance(points[i].x, points[i].y, 
                                                    clusters[0].centroid_x, clusters[0].centroid_y);
            
            for (int j = 1; j < num_clusters; j++) {
                double dist = calculate_distance(points[i].x, points[i].y, 
                                               clusters[j].centroid_x, clusters[j].centroid_y);
                if (dist < best_distance) {
                    best_distance = dist;
                    best_cluster = j;
                }
            }
            
            clusters[best_cluster].points[clusters[best_cluster].count] = points[i];
            clusters[best_cluster].count++;
        }
        
        for (int i = 0; i < num_clusters; i++) {
            if (clusters[i].count == 0) continue;
            
            double sum_x = 0.0;
            double sum_y = 0.0;
            
            for (int j = 0; j < clusters[i].count; j++) {
                sum_x += clusters[i].points[j].x;
                sum_y += clusters[i].points[j].y;
            }
            
            double new_centroid_x = sum_x / clusters[i].count;
            double new_centroid_y = sum_y / clusters[i].count;
            
            if (fabs(new_centroid_x - clusters[i].centroid_x) > 0.001 || 
                fabs(new_centroid_y - clusters[i].centroid_y) > 0.001) {
                changed = 1;
            }
            
            clusters[i].centroid_x = new_centroid_x;
            clusters[i].centroid_y = new_centroid_y;
        }
        
        if (!changed) break;
    }
    
    printf("\nClustering results:\n");
    for (int i = 0; i < num_clusters; i++) {
        printf("Cluster %d: centroid (%.3f, %.3f), %d points\n", 
               i + 1, clusters[i].centroid_x, clusters[i].centroid_y, clusters[i].count);
    }
    
    for (int i = 0; i < num_clusters; i++) {
        free(clusters[i].points);
    }
    free(clusters);
    
    return 0;
}