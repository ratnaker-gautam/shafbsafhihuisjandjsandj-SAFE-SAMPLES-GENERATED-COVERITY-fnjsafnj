//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_POINTS 1000
#define MAX_DIMENSIONS 10

struct Point {
    double *coordinates;
    int dimensions;
};

struct PointArray {
    struct Point *points;
    int count;
    int dimensions;
};

int validate_dimensions(int dim) {
    return dim > 0 && dim <= MAX_DIMENSIONS;
}

int validate_point_count(int count) {
    return count > 0 && count <= MAX_POINTS;
}

double calculate_euclidean_distance(const struct Point *p1, const struct Point *p2) {
    if (p1 == NULL || p2 == NULL || p1->dimensions != p2->dimensions) {
        return -1.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < p1->dimensions; i++) {
        double diff = p1->coordinates[i] - p2->coordinates[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

int initialize_point_array(struct PointArray *array, int count, int dimensions) {
    if (array == NULL || !validate_point_count(count) || !validate_dimensions(dimensions)) {
        return 0;
    }
    
    array->points = malloc(count * sizeof(struct Point));
    if (array->points == NULL) {
        return 0;
    }
    
    array->count = count;
    array->dimensions = dimensions;
    
    for (int i = 0; i < count; i++) {
        array->points[i].coordinates = malloc(dimensions * sizeof(double));
        if (array->points[i].coordinates == NULL) {
            for (int j = 0; j < i; j++) {
                free(array->points[j].coordinates);
            }
            free(array->points);
            return 0;
        }
        array->points[i].dimensions = dimensions;
    }
    
    return 1;
}

void free_point_array(struct PointArray *array) {
    if (array == NULL || array->points == NULL) {
        return;
    }
    
    for (int i = 0; i < array->count; i++) {
        if (array->points[i].coordinates != NULL) {
            free(array->points[i].coordinates);
        }
    }
    free(array->points);
}

int read_point_data(struct PointArray *array) {
    if (array == NULL || array->points == NULL) {
        return 0;
    }
    
    printf("Enter %d points with %d dimensions each:\n", array->count, array->dimensions);
    
    for (int i = 0; i < array->count; i++) {
        printf("Point %d: ", i + 1);
        
        for (int j = 0; j < array->dimensions; j++) {
            if (scanf("%lf", &array->points[i].coordinates[j]) != 1) {
                return 0;
            }
        }
    }
    
    return 1;
}

void compute_pairwise_distances(const struct PointArray *array) {
    if (array == NULL || array->points == NULL || array->count < 2) {
        return;
    }
    
    printf("\nPairwise Euclidean distances:\n");
    
    for (int i = 0; i < array->count; i++) {
        for (int j = i + 1; j < array->count; j++) {
            double distance = calculate_euclidean_distance(&array->points[i], &array->points[j]);
            if (distance >= 0.0) {
                printf("Distance between point %d and point %d: %.6f\n", i + 1, j + 1, distance);
            }
        }
    }
}

int main() {
    int point_count, dimensions;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1 || !validate_point_count(point_count)) {
        printf("Invalid point count\n");
        return 1;
    }
    
    printf("Enter number of dimensions (1-%d): ", MAX_DIMENSIONS);
    if (scanf("%d", &dimensions) != 1 || !validate_dimensions(dimensions)) {
        printf("Invalid dimensions\n");
        return 1;
    }
    
    struct PointArray point_array;
    
    if (!initialize_point_array(&point_array, point_count, dimensions)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (!read_point_data(&point_array)) {
        printf("Error reading point data\n");
        free_point_array(&point_array);
        return 1;
    }
    
    compute_pairwise_distances(&point_array);
    
    free_point_array(&point_array);
    return 0;
}