//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType { ADJ_MATRIX, ADJ_LIST };
enum Algorithm { BFS, DFS, DIJKSTRA };

struct Graph {
    enum GraphType type;
    int vertices;
    union {
        int **matrix;
        struct Node **list;
    } representation;
};

struct Node {
    int vertex;
    int weight;
    struct Node *next;
};

struct Graph *create_graph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->type = type;
    graph->vertices = vertices;
    
    switch (type) {
        case ADJ_MATRIX:
            graph->representation.matrix = malloc(vertices * sizeof(int *));
            if (!graph->representation.matrix) {
                free(graph);
                return NULL;
            }
            for (int i = 0; i < vertices; i++) {
                graph->representation.matrix[i] = calloc(vertices, sizeof(int));
                if (!graph->representation.matrix[i]) {
                    for (int j = 0; j < i; j++) free(graph->representation.matrix[j]);
                    free(graph->representation.matrix);
                    free(graph);
                    return NULL;
                }
            }
            break;
            
        case ADJ_LIST:
            graph->representation.list = calloc(vertices, sizeof(struct Node *));
            if (!graph->representation.list) {
                free(graph);
                return NULL;
            }
            break;
    }
    
    return graph;
}

void add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return;
    if (weight < 0) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            graph->representation.matrix[src][dest] = weight;
            break;
            
        case ADJ_LIST:
            struct Node *new_node = malloc(sizeof(struct Node));
            if (!new_node) return;
            new_node->vertex = dest;
            new_node->weight = weight;
            new_node->next = graph->representation.list[src];
            graph->representation.list[src] = new_node;
            break;
    }
}

void bfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    
    int *queue = malloc(graph->vertices * sizeof(int));
    if (!queue) {
        free(visited);
        return;
    }
    
    int front = 0, rear = 0;
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal: ");
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        switch (graph->type) {
            case ADJ_MATRIX:
                for (int i = 0; i < graph->vertices; i++) {
                    if (graph->representation.matrix[current][i] && !visited[i]) {
                        visited[i] = 1;
                        queue[rear++] = i;
                    }
                }
                break;
                
            case ADJ_LIST:
                struct Node *temp = graph->representation.list[current];
                while (temp) {
                    if (!visited[temp->vertex]) {
                        visited[temp->vertex] = 1;
                        queue[rear++] = temp->vertex;
                    }
                    temp = temp->next;
                }
                break;
        }
    }
    printf("\n");
    
    free(visited);
    free(queue);
}

void dfs_util(struct Graph *graph, int vertex, int *visited) {
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                if (graph->representation.matrix[vertex][i] && !visited[i]) {
                    dfs_util(graph, i, visited);
                }
            }
            break;
            
        case ADJ_LIST:
            struct Node *temp = graph->representation.list[vertex];
            while (temp) {
                if (!visited[temp->vertex]) {
                    dfs_util(graph, temp->vertex, visited);
                }
                temp = temp->next;
            }
            break;
    }
}

void dfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    
    printf("DFS traversal: ");
    dfs_util(graph, start, visited);
    printf("\n");
    
    free(visited);
}

void free_graph(struct Graph *graph) {
    if (!graph) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                free(graph->representation.matrix[i]);
            }
            free(graph->representation.matrix);
            break;
            
        case ADJ_LIST:
            for (int i = 0; i < graph->vertices; i++) {
                struct Node *current = graph->representation.list[i];
                while