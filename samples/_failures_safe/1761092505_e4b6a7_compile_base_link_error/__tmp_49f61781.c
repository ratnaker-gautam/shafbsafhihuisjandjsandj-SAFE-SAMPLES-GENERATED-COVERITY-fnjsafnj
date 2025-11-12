//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: numeric_computation
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

struct PointArray* create_point_array(int point_count, int dimensions) {
    if (!validate_point_count(point_count) || !validate_dimensions(dimensions)) {
        return NULL;
    }
    
    struct PointArray *array = malloc(sizeof(struct PointArray));
    if (array == NULL) {
        return NULL;
    }
    
    array->points = malloc(sizeof(struct Point) * point_count);
    if (array->points == NULL) {
        free(array);
        return NULL;
    }
    
    array->count = point_count;
    array->dimensions = dimensions;
    
    for (int i = 0; i < point_count; i++) {
        array->points[i].coordinates = malloc(sizeof(double) * dimensions);
        if (array->points[i].coordinates == NULL) {
            for (int j = 0; j < i; j++) {
                free(array->points[j].coordinates);
            }
            free(array->points);
            free(array);
            return NULL;
        }
        array->points[i].dimensions = dimensions;
    }
    
    return array;
}

void free_point_array(struct PointArray *array) {
    if (array == NULL) return;
    
    for (int i = 0; i < array->count; i++) {
        free(array->points[i].coordinates);
    }
    free(array->points);
    free(array);
}

void initialize_random_points(struct PointArray *array) {
    if (array == NULL) return;
    
    for (int i = 0; i < array->count; i++) {
        for (int j = 0; j < array->dimensions; j++) {
            array->points[i].coordinates[j] = (double)(rand() % 1000) / 100.0;
        }
    }
}

int main() {
    int point_count, dimensions;
    
    printf("Enter number of points (1-%d): ", MAX_POINTS);
    if (scanf("%d", &point_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of dimensions (1-%d): ", MAX_DIMENSIONS);
    if (scanf("%d", &dimensions) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_point_count(point_count) || !validate_dimensions(dimensions)) {
        printf("Invalid parameters\n");
        return 1;
    }
    
    struct PointArray *points = create_point_array(point_count, dimensions);
    if (points == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    srand(42);
    initialize_random_points(points);
    
    double distances[MAX_POINTS][MAX_POINTS];
    for (int i = 0; i < point_count; i++) {
        for (int j = i + 1; j < point_count; j++) {
            distances[i][j] = calculate_euclidean_distance(&points->points[i], &points->points[j]);
        }
    }
    
    printf("\nDistance matrix (upper triangle):\n");
    for (int i = 0; i < point_count; i++) {
        for (int j = 0; j < point_count; j++) {
            if (j > i) {
                printf("%8.3f ", distances[i][j]);
            } else {
                printf("%8s ", "-");
            }
        }
        printf("\n");
    }
    
    double min_distance = -1.0;
    int min_i = -1, min_j = -1;
    
    for (int i = 0; i < point_count; i++) {
        for (int j = i + 1; j < point_count; j++) {
            if (distances[i][j] >= 0.0 && (min_distance < 0.0 || distances[i][j] < min_distance)) {
                min_distance = distances[i][j];
                min_i = i;
                min_j = j;
            }
        }
    }
    
    if (min_distance >= 0.0) {
        printf("\nClosest points: %d and %d\n", min_i, min_j);
        printf("Minimum distance: %.6f\n", min_distance);
    }
    
    free_point_array(points);
    return 0;
}