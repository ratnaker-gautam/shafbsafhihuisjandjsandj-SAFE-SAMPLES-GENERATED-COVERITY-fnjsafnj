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

int read_points_from_input(struct Point** points, int* point_count, int* dimension) {
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", point_count) != 1) return 0;
    if (*point_count < 1 || *point_count > MAX_POINTS) return 0;
    
    printf("Enter dimension (1-%d): ", MAX_DIMENSIONS);
    if (scanf("%d", dimension) != 1) return 0;
    if (*dimension < 1 || *dimension > MAX_DIMENSIONS) return 0;
    
    *points = malloc(sizeof(struct Point) * (*point_count));
    if (*points == NULL) return 0;
    
    for (int i = 0; i < *point_count; i++) {
        printf("Enter %d coordinates for point %d: ", *dimension, i + 1);
        (*points)[i].dimension = *dimension;
        for (int j = 0; j < *dimension; j++) {
            if (scanf("%lf", &(*points)[i].coordinates[j]) != 1) {
                free(*points);
                return 0;
            }
        }
    }
    return 1;
}

void initialize_clusters(struct Cluster* clusters, int cluster_count, const struct Point* points, int point_count, int dimension) {
    for (int i = 0; i < cluster_count; i++) {
        clusters[i].centroid.dimension = dimension;
        clusters[i].point_count = 0;
        clusters[i].total_distance = 0.0;
        
        int point_idx = i % point_count;
        for (int j = 0; j < dimension; j++) {
            clusters[i].centroid.coordinates[j] = points[point_idx].coordinates[j];
        }
    }
}

int assign_points_to_clusters(const struct Point* points, int point_count, struct Cluster* clusters, int cluster_count) {
    int changed = 0;
    for (int i = 0; i < point_count; i++) {
        int best_cluster = 0;
        double best_distance = calculate_distance(&points[i], &clusters[0].centroid);
        if (best_distance < 0) continue;
        
        for (int j = 1; j < cluster_count; j++) {
            double distance = calculate_distance(&points[i], &clusters[j].centroid);
            if (distance >= 0 && distance < best_distance) {
                best_distance = distance;
                best_cluster = j;
            }
        }
        
        clusters[best_cluster].point_count++;
        clusters[best_cluster].total_distance += best_distance;
        changed++;
    }
    return changed > 0;
}

void update_cluster_centroids(struct Cluster* clusters, int cluster_count, const struct Point* points, int point_count) {
    for (int i = 0; i < cluster_count; i++) {
        if (clusters[i].point_count == 0) continue;
        
        for (int j = 0; j < clusters[i].centroid.dimension; j++) {
            clusters[i].centroid.coordinates[j] = 0.0;
        }
        
        int assigned_count = 0;
        for (int k = 0; k < point_count; k++) {
            double distance = calculate_distance(&points[k], &clusters[i].centroid);
            if (distance >= 0 && distance <= clusters[i].total_distance / clusters[i].point_count) {
                for (int j = 0; j < clusters[i].centroid.dimension; j++) {
                    clusters[i].centroid.coordinates[j] += points[k].coordinates[j];
                }
                assigned_count++;
            }
        }
        
        if (assigned_count > 0) {
            for (int j = 0; j < clusters[i].centroid.dimension; j++) {
                clusters[i].centroid.coordinates[j] /= assigned_count;
            }
        }
    }
}

void print_clusters(const struct Cluster* clusters, int cluster_count) {
    for (int i = 0; i < cluster_count; i++) {
        printf("Cluster %d: points=%d, total_distance=%.2f, centroid=(", i, clusters[i].point_count, clusters[i].total_distance);
        for (int j = 0; j < clusters[i].centroid.dimension; j++) {
            printf("%.2f", clusters[i].centroid.coordinates[j]);
            if (j < clusters[i].centroid.dimension - 1) printf(", ");
        }
        printf(")\n");
    }
}

int main(void) {
    struct Point* points = NULL;
    int point_count, dimension;