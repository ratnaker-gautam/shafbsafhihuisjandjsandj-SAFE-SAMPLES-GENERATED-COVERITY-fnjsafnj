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
    SHORTEST_PATH,
    ALGORITHM_COUNT
};

struct Graph {
    enum GraphType type;
    int vertex_count;
    union {
        int **matrix;
        struct Node **list;
    } data;
};

struct Node {
    int vertex;
    struct Node *next;
};

struct Graph *create_graph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->type = type;
    graph->vertex_count = vertices;
    
    switch (type) {
        case ADJ_MATRIX:
            graph->data.matrix = malloc(vertices * sizeof(int *));
            if (!graph->data.matrix) {
                free(graph);
                return NULL;
            }
            for (int i = 0; i < vertices; i++) {
                graph->data.matrix[i] = calloc(vertices, sizeof(int));
                if (!graph->data.matrix[i]) {
                    for (int j = 0; j < i; j++) free(graph->data.matrix[j]);
                    free(graph->data.matrix);
                    free(graph);
                    return NULL;
                }
            }
            break;
            
        case ADJ_LIST:
            graph->data.list = calloc(vertices, sizeof(struct Node *));
            if (!graph->data.list) {
                free(graph);
                return NULL;
            }
            break;
            
        default:
            free(graph);
            return NULL;
    }
    
    return graph;
}

void add_edge(struct Graph *graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->vertex_count || 
        dest < 0 || dest >= graph->vertex_count) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            graph->data.matrix[src][dest] = 1;
            graph->data.matrix[dest][src] = 1;
            break;
            
        case ADJ_LIST:
            {
                struct Node *new_node = malloc(sizeof(struct Node));
                if (!new_node) return;
                new_node->vertex = dest;
                new_node->next = graph->data.list[src];
                graph->data.list[src] = new_node;
                
                new_node = malloc(sizeof(struct Node));
                if (!new_node) return;
                new_node->vertex = src;
                new_node->next = graph->data.list[dest];
                graph->data.list[dest] = new_node;
            }
            break;
            
        default:
            break;
    }
}

void bfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertex_count) return;
    
    int *visited = calloc(graph->vertex_count, sizeof(int));
    if (!visited) return;
    
    int *queue = malloc(graph->vertex_count * sizeof(int));
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
                for (int i = 0; i < graph->vertex_count; i++) {
                    if (graph->data.matrix[current][i] && !visited[i]) {
                        visited[i] = 1;
                        queue[rear++] = i;
                    }
                }
                break;
                
            case ADJ_LIST:
                {
                    struct Node *temp = graph->data.list[current];
                    while (temp) {
                        if (!visited[temp->vertex]) {
                            visited[temp->vertex] = 1;
                            queue[rear++] = temp->vertex;
                        }
                        temp = temp->next;
                    }
                }
                break;
                
            default:
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
            for (int i = 0; i < graph->vertex_count; i++) {
                if (graph->data.matrix[vertex][i] && !visited[i]) {
                    dfs_util(graph, i, visited);
                }
            }
            break;
            
        case ADJ_LIST:
            {
                struct Node *temp = graph->data.list[vertex];
                while (temp) {
                    if (!visited[temp->vertex]) {
                        dfs_util(graph, temp->vertex, visited);
                    }
                    temp = temp->next;
                }
            }
            break;
            
        default:
            break;
    }
}

void dfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertex_count) return;
    
    int *visited = calloc(graph->vertex_count, sizeof(int));
    if (!visited) return;
    
    printf("DFS traversal: ");
    dfs_util(graph, start, visited);
    printf