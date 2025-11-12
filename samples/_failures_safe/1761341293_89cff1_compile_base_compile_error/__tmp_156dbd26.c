//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_DIMENSIONS 10

struct Point {
    double coordinates[MAX_DIMENSIONS];
    int dimension;
};

struct Cluster {
    struct Point centroid;
    int point_count;
    double total_distance;
};

double calculate_distance(const struct Point* p1, const struct Point* p2) {
    if (p1->dimension != p2->dimension) return -1.0;
    double sum = 0.0;
    for (int i = 0; i < p1->dimension; i++) {
        double diff = p1->coordinates[i] - p2->coordinates[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

int read_points_from_input(struct Point* points, int max_points) {
    int point_count, dimension;
    if (scanf("%d %d", &point_count, &dimension) != 2) return -1;
    if (point_count <= 0 || point_count > max_points) return -1;
    if (dimension <= 0 || dimension > MAX_DIMENSIONS) return -1;
    
    for (int i = 0; i < point_count; i++) {
        points[i].dimension = dimension;
        for (int j = 0; j < dimension; j++) {
            if (scanf("%lf", &points[i].coordinates[j]) != 1) return -1;
        }
    }
    return point_count;
}

void initialize_centroids(struct Cluster* clusters, int cluster_count, const struct Point* points, int point_count) {
    for (int i = 0; i < cluster_count; i++) {
        int point_index = i % point_count;
        clusters[i].centroid.dimension = points[point_index].dimension;
        memcpy(clusters[i].centroid.coordinates, points[point_index].coordinates, 
               sizeof(double) * points[point_index].dimension);
        clusters[i].point_count = 0;
        clusters[i].total_distance = 0.0;
    }
}

int assign_points_to_clusters(struct Point* points, int point_count, 
                             struct Cluster* clusters, int cluster_count) {
    int changes = 0;
    for (int i = 0; i < point_count; i++) {
        int best_cluster = 0;
        double best_distance = calculate_distance(&points[i], &clusters[0].centroid);
        if (best_distance < 0) return -1;
        
        for (int j = 1; j < cluster_count; j++) {
            double distance = calculate_distance(&points[i], &clusters[j].centroid);
            if (distance < 0) return -1;
            if (distance < best_distance) {
                best_distance = distance;
                best_cluster = j;
            }
        }
        
        if (points[i].dimension != clusters[best_cluster].centroid.dimension) return -1;
        clusters[best_cluster].point_count++;
        clusters[best_cluster].total_distance += best_distance;
        changes++;
    }
    return changes;
}

void update_centroids(struct Cluster* clusters, int cluster_count) {
    for (int i = 0; i < cluster_count; i++) {
        if (clusters[i].point_count > 0) {
            for (int j = 0; j < clusters[i].centroid.dimension; j++) {
                clusters[i].centroid.coordinates[j] = 
                    clusters[i].centroid.coordinates[j] * 0.9 + 
                    (clusters[i].total_distance / clusters[i].point_count) * 0.1;
            }
        }
        clusters[i].point_count = 0;
        clusters[i].total_distance = 0.0;
    }
}

int main(void) {
    struct Point points[MAX_POINTS];
    int point_count = read_points_from_input(points, MAX_POINTS);
    if (point_count <= 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int cluster_count;
    if (scanf("%d", &cluster_count) != 1) {
        fprintf(stderr, "Invalid cluster count\n");
        return 1;
    }
    if (cluster_count <= 0 || cluster_count > 10) {
        fprintf(stderr, "Invalid cluster count\n");
        return 1;
    }
    
    struct Cluster* clusters = malloc(sizeof(struct Cluster) * cluster_count);
    if (!clusters) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    initialize_centroids(clusters, cluster_count, points, point_count);
    
    for (int iteration = 0; iteration < 20; iteration++) {
        int changes = assign_points_to_clusters(points, point_count, clusters, cluster_count);
        if (changes < 0) {
            fprintf(stderr, "Error in cluster assignment\n");
            free(clusters);
            return 1;
        }
        update_centroids(clusters, cluster_count);
    }
    
    printf("Final centroids:\n");
    for (int i = 0; i < cluster_count; i++) {
        printf("Cluster %d: ", i + 1);
        for (int j = 0; j < clusters[i].centroid.dimension; j++) {
            printf("%.2f ", clusters[i].centroid.coordinates[j]);
        }
        printf("\n");
    }
    
    free(cl