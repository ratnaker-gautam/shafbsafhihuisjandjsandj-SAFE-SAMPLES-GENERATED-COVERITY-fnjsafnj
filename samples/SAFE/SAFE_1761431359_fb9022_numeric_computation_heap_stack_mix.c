//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 1000

struct Point {
    double x;
    double y;
};

struct Cluster {
    double centroid_x;
    double centroid_y;
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
    
    struct Cluster clusters[10];
    
    printf("Enter %d points (x y):\n", num_points);
    for (int i = 0; i < num_points; i++) {
        if (scanf("%lf %lf", &points[i].x, &points[i].y) != 2) {
            fprintf(stderr, "Invalid point input\n");
            free(points);
            return 1;
        }
    }
    
    for (int i = 0; i < num_clusters; i++) {
        clusters[i].centroid_x = points[i].x;
        clusters[i].centroid_y = points[i].y;
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
            int closest_cluster = 0;
            double min_distance = calculate_distance(points[i].x, points[i].y, 
                                                   clusters[0].centroid_x, clusters[0].centroid_y);
            
            for (int j = 1; j < num_clusters; j++) {
                double dist = calculate_distance(points[i].x, points[i].y,
                                               clusters[j].centroid_x, clusters[j].centroid_y);
                if (dist < min_distance) {
                    min_distance = dist;
                    closest_cluster = j;
                }
            }
            
            clusters[closest_cluster].point_count++;
        }
        
        for (int i = 0; i < num_clusters; i++) {
            if (clusters[i].point_count == 0) continue;
            
            double sum_x = 0.0;
            double sum_y = 0.0;
            int count = 0;
            
            for (int j = 0; j < num_points; j++) {
                double dist = calculate_distance(points[j].x, points[j].y,
                                               clusters[i].centroid_x, clusters[i].centroid_y);
                int is_closest = 1;
                for (int k = 0; k < num_clusters; k++) {
                    if (k == i) continue;
                    double other_dist = calculate_distance(points[j].x, points[j].y,
                                                         clusters[k].centroid_x, clusters[k].centroid_y);
                    if (other_dist < dist) {
                        is_closest = 0;
                        break;
                    }
                }
                
                if (is_closest) {
                    sum_x += points[j].x;
                    sum_y += points[j].y;
                    count++;
                }
            }
            
            if (count > 0) {
                double new_centroid_x = sum_x / count;
                double new_centroid_y = sum_y / count;
                
                if (fabs(new_centroid_x - clusters[i].centroid_x) > 1e-6 ||
                    fabs(new_centroid_y - clusters[i].centroid_y) > 1e-6) {
                    changed = 1;
                }
                
                clusters[i].centroid_x = new_centroid_x;
                clusters[i].centroid_y = new_centroid_y;
            }
        }
        
        if (!changed) {
            break;
        }
    }
    
    printf("\nFinal cluster centroids:\n");
    for (int i = 0; i < num_clusters; i++) {
        printf("Cluster %d: (%.3f, %.3f) - %d points\n", 
               i + 1, clusters[i].centroid_x, clusters[i].centroid_y, clusters[i].point_count);
    }
    
    free(points);
    return 0;
}