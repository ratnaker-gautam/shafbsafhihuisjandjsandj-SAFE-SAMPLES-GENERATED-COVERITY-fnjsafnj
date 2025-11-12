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
    void *data;
};

struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode *next;
};

struct AdjList {
    struct AdjListNode *head;
};

int validate_vertex(struct Graph *graph, int vertex) {
    return graph && (vertex >= 0 && vertex < graph->vertices);
}

void bfs_matrix(int **matrix, int vertices, int start) {
    if (!matrix || vertices <= 0 || start < 0 || start >= vertices) return;
    
    int *visited = calloc(vertices, sizeof(int));
    if (!visited) return;
    
    int *queue = malloc(vertices * sizeof(int));
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
        
        for (int i = 0; i < vertices; i++) {
            if (matrix[current][i] && !visited[i]) {
                visited[i] = 1;
                if (rear < vertices) {
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("\n");
    
    free(visited);
    free(queue);
}

void dfs_list(struct AdjList *list, int vertices, int start) {
    if (!list || vertices <= 0 || start < 0 || start >= vertices) return;
    
    int *visited = calloc(vertices, sizeof(int));
    if (!visited) return;
    
    int *stack = malloc(vertices * sizeof(int));
    if (!stack) {
        free(visited);
        return;
    }
    
    int top = 0;
    stack[top++] = start;
    
    printf("DFS traversal: ");
    while (top > 0) {
        int current = stack[--top];
        
        if (!visited[current]) {
            visited[current] = 1;
            printf("%d ", current);
            
            struct AdjListNode *node = list[current].head;
            while (node) {
                if (!visited[node->dest] && node->dest >= 0 && node->dest < vertices) {
                    if (top < vertices) {
                        stack[top++] = node->dest;
                    }
                }
                node = node->next;
            }
        }
    }
    printf("\n");
    
    free(visited);
    free(stack);
}

void dijkstra_matrix(int **matrix, int vertices, int start) {
    if (!matrix || vertices <= 0 || start < 0 || start >= vertices) return;
    
    int *dist = malloc(vertices * sizeof(int));
    if (!dist) return;
    
    int *visited = calloc(vertices, sizeof(int));
    if (!visited) {
        free(dist);
        return;
    }
    
    for (int i = 0; i < vertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (int count = 0; count < vertices - 1; count++) {
        int min_dist = INT_MAX, min_index = -1;
        
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && matrix[min_index][v] && dist[min_index] != INT_MAX) {
                long new_dist = (long)dist[min_index] + matrix[min_index][v];
                if (new_dist < dist[v] && new_dist < INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from vertex %d:\n", start);
    for (int i = 0; i < vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("Vertex %d: unreachable\n", i);
        } else {
            printf("Vertex %d: %d\n", i, dist[i]);
        }
    }
    
    free(dist);
    free(visited);
}

void execute_algorithm(struct Graph *graph, enum Algorithm algo, int start_vertex) {
    if (!graph || !validate_vertex(graph, start_vertex)) return;
    
    switch (algo) {
        case BFS:
            if (graph->type == ADJ_MATRIX) {
                bfs_matrix((int **)graph->data, graph->vertices, start_vertex);
            }
            break;
        case DFS:
            if (graph->type == ADJ_LIST) {
                dfs_list((struct AdjList *)graph->data, graph->vertices, start_vertex);
            }
            break;
        case DIJKSTRA:
            if (graph->type == ADJ_MATRIX) {
                dijkstra_matrix((int **)graph->data, graph->vertices, start_vertex);
            }
            break;
    }
}

void cleanup_graph(struct Graph *graph)