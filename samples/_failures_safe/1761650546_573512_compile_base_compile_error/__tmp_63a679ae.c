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

enum Algorithm {
    BFS,
    DFS,
    DIJKSTRA,
    ALGORITHM_COUNT
};

struct Graph {
    enum GraphType type;
    int vertex_count;
    int **adj_matrix;
    int **adj_list;
    int *list_sizes;
};

int validate_vertex_count(int count) {
    return count > 0 && count <= 100;
}

int validate_weight(int weight) {
    return weight >= 0 && weight <= 1000;
}

struct Graph *create_graph(enum GraphType type, int vertex_count) {
    if (!validate_vertex_count(vertex_count)) {
        return NULL;
    }
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->type = type;
    graph->vertex_count = vertex_count;
    graph->adj_matrix = NULL;
    graph->adj_list = NULL;
    graph->list_sizes = NULL;
    
    switch (type) {
        case ADJ_MATRIX:
            graph->adj_matrix = malloc(vertex_count * sizeof(int *));
            if (!graph->adj_matrix) {
                free(graph);
                return NULL;
            }
            for (int i = 0; i < vertex_count; i++) {
                graph->adj_matrix[i] = calloc(vertex_count, sizeof(int));
                if (!graph->adj_matrix[i]) {
                    for (int j = 0; j < i; j++) {
                        free(graph->adj_matrix[j]);
                    }
                    free(graph->adj_matrix);
                    free(graph);
                    return NULL;
                }
            }
            break;
            
        case ADJ_LIST:
            graph->adj_list = malloc(vertex_count * sizeof(int *));
            graph->list_sizes = calloc(vertex_count, sizeof(int));
            if (!graph->adj_list || !graph->list_sizes) {
                free(graph->adj_list);
                free(graph->list_sizes);
                free(graph);
                return NULL;
            }
            for (int i = 0; i < vertex_count; i++) {
                graph->adj_list[i] = malloc(vertex_count * sizeof(int));
                if (!graph->adj_list[i]) {
                    for (int j = 0; j < i; j++) {
                        free(graph->adj_list[j]);
                    }
                    free(graph->adj_list);
                    free(graph->list_sizes);
                    free(graph);
                    return NULL;
                }
            }
            break;
            
        default:
            free(graph);
            return NULL;
    }
    
    return graph;
}

void add_edge(struct Graph *graph, int from, int to, int weight) {
    if (!graph || !validate_weight(weight)) return;
    if (from < 0 || from >= graph->vertex_count) return;
    if (to < 0 || to >= graph->vertex_count) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            graph->adj_matrix[from][to] = weight;
            break;
            
        case ADJ_LIST:
            if (graph->list_sizes[from] < graph->vertex_count) {
                graph->adj_list[from][graph->list_sizes[from]] = to;
                graph->list_sizes[from]++;
            }
            break;
            
        default:
            break;
    }
}

void dfs_util(struct Graph *graph, int vertex, int *visited) {
    if (!graph || !visited) return;
    
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertex_count; i++) {
                if (graph->adj_matrix[vertex][i] != 0 && !visited[i]) {
                    dfs_util(graph, i, visited);
                }
            }
            break;
            
        case ADJ_LIST:
            for (int i = 0; i < graph->list_sizes[vertex]; i++) {
                int neighbor = graph->adj_list[vertex][i];
                if (!visited[neighbor]) {
                    dfs_util(graph, neighbor, visited);
                }
            }
            break;
            
        default:
            break;
    }
}

void dfs_traversal(struct Graph *graph, int start_vertex) {
    if (!graph || start_vertex < 0 || start_vertex >= graph->vertex_count) return;
    
    int *visited = calloc(graph->vertex_count, sizeof(int));
    if (!visited) return;
    
    printf("DFS traversal: ");
    dfs_util(graph, start_vertex, visited);
    printf("\n");
    
    free(visited);
}

void bfs_traversal(struct Graph *graph, int start_vertex) {
    if (!graph || start_vertex < 0 || start_vertex >= graph->vertex_count) return;
    
    int *visited = calloc(graph->vertex_count, sizeof(int));
    if (!visited) return;
    
    int *queue = malloc(graph->vertex_count * sizeof(int));
    if (!queue) {
        free(visited);
        return;
    }
    
    int front = 0, rear = 0;
    visited[start_vertex] = 1;
    queue[rear++] = start_vertex;
    
    printf("BFS traversal: ");
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d