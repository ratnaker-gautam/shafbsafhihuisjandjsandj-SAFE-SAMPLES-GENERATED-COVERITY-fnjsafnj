//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_CLUSTERS 10

struct Point {
    double x;
    double y;
    int cluster;
};

struct Cluster {
    double center_x;
    double center_y;
    int point_count;
};

double calculate_distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

int read_points_from_input(struct Point* points, int max_points) {
    int count = 0;
    char buffer[256];
    
    printf("Enter points (x y), one per line. Enter 'done' to finish:\n");
    
    while (count < max_points) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (strncmp(buffer, "done", 4) == 0) {
            break;
        }
        
        double x, y;
        if (sscanf(buffer, "%lf %lf", &x, &y) == 2) {
            points[count].x = x;
            points[count].y = y;
            points[count].cluster = -1;
            count++;
        } else {
            printf("Invalid input. Please enter two numbers or 'done'.\n");
        }
    }
    
    return count;
}

void initialize_clusters(struct Cluster* clusters, int k, struct Point* points, int point_count) {
    if (point_count == 0 || k == 0) return;
    
    for (int i = 0; i < k; i++) {
        int idx = (i * point_count) / k;
        if (idx >= point_count) idx = point_count - 1;
        clusters[i].center_x = points[idx].x;
        clusters[i].center_y = points[idx].y;
        clusters[i].point_count = 0;
    }
}

int assign_points_to_clusters(struct Point* points, int point_count, struct Cluster* clusters, int k) {
    int changed = 0;
    
    for (int i = 0; i < point_count; i++) {
        double min_dist = -1.0;
        int best_cluster = -1;
        
        for (int j = 0; j < k; j++) {
            double dist = calculate_distance(points[i].x, points[i].y, 
                                           clusters[j].center_x, clusters[j].center_y);
            
            if (best_cluster == -1 || dist < min_dist) {
                min_dist = dist;
                best_cluster = j;
            }
        }
        
        if (points[i].cluster != best_cluster) {
            points[i].cluster = best_cluster;
            changed = 1;
        }
    }
    
    return changed;
}

void update_cluster_centers(struct Cluster* clusters, int k, struct Point* points, int point_count) {
    double* sum_x = calloc(k, sizeof(double));
    double* sum_y = calloc(k, sizeof(double));
    int* counts = calloc(k, sizeof(int));
    
    if (sum_x == NULL || sum_y == NULL || counts == NULL) {
        free(sum_x);
        free(sum_y);
        free(counts);
        return;
    }
    
    for (int i = 0; i < point_count; i++) {
        int cluster = points[i].cluster;
        if (cluster >= 0 && cluster < k) {
            sum_x[cluster] += points[i].x;
            sum_y[cluster] += points[i].y;
            counts[cluster]++;
        }
    }
    
    for (int i = 0; i < k; i++) {
        if (counts[i] > 0) {
            clusters[i].center_x = sum_x[i] / counts[i];
            clusters[i].center_y = sum_y[i] / counts[i];
            clusters[i].point_count = counts[i];
        }
    }
    
    free(sum_x);
    free(sum_y);
    free(counts);
}

int main() {
    struct Point points_stack[MAX_POINTS];
    struct Point* points_heap = NULL;
    int point_count;
    
    printf("K-means Clustering Demo\n");
    printf("Maximum points: %d\n", MAX_POINTS);
    
    point_count = read_points_from_input(points_stack, MAX_POINTS);
    
    if (point_count == 0) {
        printf("No points entered. Exiting.\n");
        return 1;
    }
    
    if (point_count > 500) {
        points_heap = malloc(point_count * sizeof(struct Point));
        if (points_heap == NULL) {
            printf("Memory allocation failed. Using stack storage.\n");
            points_heap = points_stack;
        } else {
            memcpy(points_heap, points_stack, point_count * sizeof(struct Point));
        }
    } else {
        points_heap = points_stack;
    }
    
    int k;
    printf("Enter number of clusters (2-%d): ", MAX_CLUSTERS);
    if (scanf("%d", &k) != 1 || k < 2 || k > MAX_CLUSTERS) {
        printf("Invalid number of clusters. Using 3.\n");
        k = 3;
    }
    
    struct Cluster* clusters = malloc(k * sizeof(struct Cluster));
    if (clusters == NULL) {
        printf("Memory allocation failed for clusters.\n");
        if