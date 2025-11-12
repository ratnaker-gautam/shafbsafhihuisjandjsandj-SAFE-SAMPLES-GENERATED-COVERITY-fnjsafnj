//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType {
    ADJ_MATRIX,
    ADJ_LIST,
    GRAPH_TYPE_COUNT
};

enum Operation {
    ADD_EDGE,
    REMOVE_EDGE,
    FIND_PATH,
    OPERATION_COUNT
};

struct Graph {
    enum GraphType type;
    int vertex_count;
    int **adj_matrix;
    int **adj_list;
    int *list_sizes;
};

int validate_vertex(struct Graph *g, int v) {
    return (g && v >= 0 && v < g->vertex_count);
}

struct Graph *create_graph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    
    g->type = type;
    g->vertex_count = vertices;
    
    switch (type) {
        case ADJ_MATRIX:
            g->adj_matrix = malloc(vertices * sizeof(int *));
            if (!g->adj_matrix) { free(g); return NULL; }
            
            for (int i = 0; i < vertices; i++) {
                g->adj_matrix[i] = calloc(vertices, sizeof(int));
                if (!g->adj_matrix[i]) {
                    for (int j = 0; j < i; j++) free(g->adj_matrix[j]);
                    free(g->adj_matrix);
                    free(g);
                    return NULL;
                }
            }
            g->adj_list = NULL;
            g->list_sizes = NULL;
            break;
            
        case ADJ_LIST:
            g->adj_list = malloc(vertices * sizeof(int *));
            g->list_sizes = calloc(vertices, sizeof(int));
            if (!g->adj_list || !g->list_sizes) {
                free(g->adj_list);
                free(g->list_sizes);
                free(g);
                return NULL;
            }
            
            for (int i = 0; i < vertices; i++) {
                g->adj_list[i] = malloc(10 * sizeof(int));
                if (!g->adj_list[i]) {
                    for (int j = 0; j < i; j++) free(g->adj_list[j]);
                    free(g->adj_list);
                    free(g->list_sizes);
                    free(g);
                    return NULL;
                }
                g->list_sizes[i] = 0;
            }
            g->adj_matrix = NULL;
            break;
            
        default:
            free(g);
            return NULL;
    }
    
    return g;
}

void free_graph(struct Graph *g) {
    if (!g) return;
    
    switch (g->type) {
        case ADJ_MATRIX:
            if (g->adj_matrix) {
                for (int i = 0; i < g->vertex_count; i++) {
                    free(g->adj_matrix[i]);
                }
                free(g->adj_matrix);
            }
            break;
        case ADJ_LIST:
            if (g->adj_list) {
                for (int i = 0; i < g->vertex_count; i++) {
                    free(g->adj_list[i]);
                }
                free(g->adj_list);
            }
            free(g->list_sizes);
            break;
        default:
            break;
    }
    free(g);
}

int dfs_find_path(struct Graph *g, int current, int target, int *visited) {
    if (current == target) return 1;
    visited[current] = 1;
    
    switch (g->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < g->vertex_count; i++) {
                if (g->adj_matrix[current][i] && !visited[i]) {
                    if (dfs_find_path(g, i, target, visited)) {
                        return 1;
                    }
                }
            }
            break;
        case ADJ_LIST:
            for (int i = 0; i < g->list_sizes[current]; i++) {
                int neighbor = g->adj_list[current][i];
                if (!visited[neighbor]) {
                    if (dfs_find_path(g, neighbor, target, visited)) {
                        return 1;
                    }
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

int graph_operation(struct Graph *g, enum Operation op, int v1, int v2) {
    if (!g || !validate_vertex(g, v1) || !validate_vertex(g, v2)) return 0;
    
    switch (op) {
        case ADD_EDGE:
            switch (g->type) {
                case ADJ_MATRIX:
                    g->adj_matrix[v1][v2] = 1;
                    g->adj_matrix[v2][v1] = 1;
                    break;
                case ADJ_LIST:
                    if (g->list_sizes[v1] < 10) {
                        g->adj_list[v1][g->list_sizes[v1]++] = v2;
                    }
                    if (g->list_sizes[v2] < 10) {
                        g->adj_list[v2][g->list_sizes[v2]++] = v1;
                    }
                    break;
                default:
                    return 0;
            }
            return 1;
            
        case REMOVE_EDGE:
            switch (g->type) {
                case ADJ_MATRIX:
                    g->adj_matrix[v1][v2] = 0;