//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum GraphType { ADJ_MATRIX, ADJ_LIST };

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

struct Graph* createGraph(enum GraphType type, int vertices) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->type = type;
    graph->vertices = vertices;
    
    if (type == ADJ_MATRIX) {
        graph->representation.matrix = malloc(vertices * sizeof(int*));
        if (!graph->representation.matrix) {
            free(graph);
            return NULL;
        }
        
        for (int i = 0; i < vertices; i++) {
            graph->representation.matrix[i] = calloc(vertices, sizeof(int));
            if (!graph->representation.matrix[i]) {
                for (int j = 0; j < i; j++) {
                    free(graph->representation.matrix[j]);
                }
                free(graph->representation.matrix);
                free(graph);
                return NULL;
            }
        }
    } else {
        graph->representation.list = calloc(vertices, sizeof(struct Node*));
        if (!graph->representation.list) {
            free(graph);
            return NULL;
        }
    }
    
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return;
    if (weight < 0 || weight > 1000) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            graph->representation.matrix[src][dest] = weight;
            break;
            
        case ADJ_LIST: {
            struct Node *newNode = malloc(sizeof(struct Node));
            if (!newNode) return;
            
            newNode->vertex = dest;
            newNode->weight = weight;
            newNode->next = graph->representation.list[src];
            graph->representation.list[src] = newNode;
            break;
        }
    }
}

void dfsUtil(struct Graph *graph, int vertex, int *visited) {
    if (!graph || !visited || vertex < 0 || vertex >= graph->vertices) return;
    
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                if (graph->representation.matrix[vertex][i] != 0 && !visited[i]) {
                    dfsUtil(graph, i, visited);
                }
            }
            break;
            
        case ADJ_LIST: {
            struct Node *current = graph->representation.list[vertex];
            while (current) {
                if (!visited[current->vertex]) {
                    dfsUtil(graph, current->vertex, visited);
                }
                current = current->next;
            }
            break;
        }
    }
}

void dfs(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    
    printf("DFS traversal: ");
    dfsUtil(graph, start, visited);
    printf("\n");
    
    free(visited);
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
                    if (graph->representation.matrix[current][i] != 0 && !visited[i]) {
                        visited[i] = 1;
                        queue[rear++] = i;
                    }
                }
                break;
                
            case ADJ_LIST: {
                struct Node *node = graph->representation.list[current];
                while (node) {
                    if (!visited[node->vertex]) {
                        visited[node->vertex] = 1;
                        queue[rear++] = node->vertex;
                    }
                    node = node->next;
                }
                break;
            }
        }
    }
    
    printf("\n");
    free(visited);
    free(queue);
}

void freeGraph(struct Graph *graph) {
    if (!graph) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            for (int i = 0; i < graph->vertices; i++) {
                free(graph->representation.matrix[i]);
            }
            free(graph->representation.matrix);
            break;
            
        case ADJ_LIST:
            for (int i = 0; i < graph->vertices; i++)