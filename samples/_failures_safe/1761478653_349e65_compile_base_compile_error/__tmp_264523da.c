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
    return (v >= 0 && v < g->vertex_count);
}

int validate_weight(int w) {
    return (w >= 0 && w <= 1000);
}

struct Graph *create_graph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *g = malloc(sizeof(struct Graph));
    if (!g) return NULL;
    
    g->type = type;
    g->vertex_count = vertices;
    g->adj_matrix = NULL;
    g->adj_list = NULL;
    g->list_sizes = NULL;
    
    if (type == ADJ_MATRIX) {
        g->adj_matrix = malloc(vertices * sizeof(int *));
        if (!g->adj_matrix) {
            free(g);
            return NULL;
        }
        
        for (int i = 0; i < vertices; i++) {
            g->adj_matrix[i] = calloc(vertices, sizeof(int));
            if (!g->adj_matrix[i]) {
                for (int j = 0; j < i; j++) free(g->adj_matrix[j]);
                free(g->adj_matrix);
                free(g);
                return NULL;
            }
        }
    } else if (type == ADJ_LIST) {
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
        }
    }
    
    return g;
}

void free_graph(struct Graph *g) {
    if (!g) return;
    
    if (g->type == ADJ_MATRIX && g->adj_matrix) {
        for (int i = 0; i < g->vertex_count; i++) free(g->adj_matrix[i]);
        free(g->adj_matrix);
    } else if (g->type == ADJ_LIST && g->adj_list) {
        for (int i = 0; i < g->vertex_count; i++) free(g->adj_list[i]);
        free(g->adj_list);
        free(g->list_sizes);
    }
    free(g);
}

int add_edge_matrix(struct Graph *g, int src, int dest, int weight) {
    if (!validate_vertex(g, src) || !validate_vertex(g, dest) || !validate_weight(weight)) return 0;
    g->adj_matrix[src][dest] = weight;
    g->adj_matrix[dest][src] = weight;
    return 1;
}

int add_edge_list(struct Graph *g, int src, int dest, int weight) {
    if (!validate_vertex(g, src) || !validate_vertex(g, dest) || !validate_weight(weight)) return 0;
    
    if (g->list_sizes[src] >= 10 || g->list_sizes[dest] >= 10) return 0;
    
    g->adj_list[src][g->list_sizes[src]++] = dest;
    g->adj_list[dest][g->list_sizes[dest]++] = src;
    return 1;
}

int remove_edge_matrix(struct Graph *g, int src, int dest) {
    if (!validate_vertex(g, src) || !validate_vertex(g, dest)) return 0;
    g->adj_matrix[src][dest] = 0;
    g->adj_matrix[dest][src] = 0;
    return 1;
}

int remove_edge_list(struct Graph *g, int src, int dest) {
    if (!validate_vertex(g, src) || !validate_vertex(g, dest)) return 0;
    
    for (int i = 0; i < g->list_sizes[src]; i++) {
        if (g->adj_list[src][i] == dest) {
            for (int j = i; j < g->list_sizes[src] - 1; j++) {
                g->adj_list[src][j] = g->adj_list[src][j + 1];
            }
            g->list_sizes[src]--;
            break;
        }
    }
    
    for (int i = 0; i < g->list_sizes[dest]; i++) {
        if (g->adj_list[dest][i] == src) {
            for (int j = i; j < g->list_sizes[dest] - 1; j++) {
                g->adj_list[dest][j] = g