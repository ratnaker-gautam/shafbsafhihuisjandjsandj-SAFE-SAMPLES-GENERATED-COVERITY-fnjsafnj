//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
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

int validate_weight(int weight) {
    return (weight >= 0 && weight <= 1000);
}

struct Graph* create_graph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    
    g->type = type;
    g->vertex_count = vertices;
    g->adj_matrix = NULL;
    g->adj_list = NULL;
    g->list_sizes = NULL;
    
    switch (type) {
        case ADJ_MATRIX:
            g->adj_matrix = malloc(vertices * sizeof(int*));
            if (!g->adj_matrix) { free(g); return NULL; }
            for (int i = 0; i < vertices; i++) {
                g->adj_matrix[i] = calloc(vertices, sizeof(int));
                if (!g->adj_matrix[i]) {
                    for (int j = 0; j < i; j++) free(g->adj_matrix[j]);
                    free(g->adj_matrix); free(g); return NULL;
                }
            }
            break;
            
        case ADJ_LIST:
            g->adj_list = malloc(vertices * sizeof(int*));
            g->list_sizes = calloc(vertices, sizeof(int));
            if (!g->adj_list || !g->list_sizes) {
                free(g->adj_list); free(g->list_sizes); free(g); return NULL;
            }
            for (int i = 0; i < vertices; i++) {
                g->adj_list[i] = malloc(10 * sizeof(int));
                if (!g->adj_list[i]) {
                    for (int j = 0; j < i; j++) free(g->adj_list[j]);
                    free(g->adj_list); free(g->list_sizes); free(g); return NULL;
                }
            }
            break;
            
        default:
            free(g);
            return NULL;
    }
    
    return g;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (!g || !validate_vertex(g, from) || !validate_vertex(g, to) || !validate_weight(weight)) return 0;
    if (from == to) return 0;
    
    switch (g->type) {
        case ADJ_MATRIX:
            g->adj_matrix[from][to] = weight;
            g->adj_matrix[to][from] = weight;
            break;
            
        case ADJ_LIST:
            if (g->list_sizes[from] >= 10 || g->list_sizes[to] >= 10) return 0;
            g->adj_list[from][g->list_sizes[from]++] = to;
            g->adj_list[to][g->list_sizes[to]++] = from;
            break;
            
        default:
            return 0;
    }
    
    return 1;
}

int find_path_dfs(struct Graph *g, int start, int end, int *visited, int *path, int depth) {
    if (!validate_vertex(g, start) || !validate_vertex(g, end)) return 0;
    if (start == end) {
        path[depth] = end;
        return depth + 1;
    }
    
    visited[start] = 1;
    path[depth] = start;
    
    switch (g->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < g->vertex_count; i++) {
                if (g->adj_matrix[start][i] > 0 && !visited[i]) {
                    int result = find_path_dfs(g, i, end, visited, path, depth + 1);
                    if (result > 0) return result;
                }
            }
            break;
            
        case ADJ_LIST:
            for (int i = 0; i < g->list_sizes[start]; i++) {
                int neighbor = g->adj_list[start][i];
                if (!visited[neighbor]) {
                    int result = find_path_dfs(g, neighbor, end, visited, path, depth + 1);
                    if (result > 0) return result;
                }
            }
            break;
            
        default:
            break;
    }
    
    visited[start] = 0;
    return 0;
}

void free_graph(struct Graph *g) {
    if (!g) return;
    
    switch (g->type) {
        case ADJ_MATRIX:
            if (g->adj_matrix) {
                for (int i = 0; i < g->vertex_count; i++) free(g->adj_matrix[i]);
                free(g->adj_matrix);
            }
            break;
            
        case ADJ_LIST:
            if (g->adj_list) {
                for (int i = 0; i < g->vertex_count; i++) free(g->adj_list[i]);