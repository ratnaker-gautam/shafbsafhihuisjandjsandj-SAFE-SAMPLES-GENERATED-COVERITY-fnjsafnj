//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
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

struct Graph *createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) return NULL;
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->vertices = vertices;
    graph->type = type;
    
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
                    for (int j = 0; j < i; j++) {
                        free(graph->representation.matrix[j]);
                    }
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

void addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return;
    if (weight < 0 || weight > 1000) return;
    
    switch (graph->type) {
        case ADJ_MATRIX:
            graph->representation.matrix[src][dest] = weight;
            graph->representation.matrix[dest][src] = weight;
            break;
            
        case ADJ_LIST:
            struct Node *newNode = malloc(sizeof(struct Node));
            if (!newNode) return;
            newNode->vertex = dest;
            newNode->weight = weight;
            newNode->next = graph->representation.list[src];
            graph->representation.list[src] = newNode;
            
            newNode = malloc(sizeof(struct Node));
            if (!newNode) return;
            newNode->vertex = src;
            newNode->weight = weight;
            newNode->next = graph->representation.list[dest];
            graph->representation.list[dest] = newNode;
            break;
    }
}

void dijkstra(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    
    int *dist = malloc(graph->vertices * sizeof(int));
    int *visited = calloc(graph->vertices, sizeof(int));
    if (!dist || !visited) {
        free(dist);
        free(visited);
        return;
    }
    
    for (int i = 0; i < graph->vertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    for (int count = 0; count < graph->vertices - 1; count++) {
        int minDist = INT_MAX;
        int minVertex = -1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minVertex = v;
            }
        }
        
        if (minVertex == -1) break;
        visited[minVertex] = 1;
        
        switch (graph->type) {
            case ADJ_MATRIX:
                for (int v = 0; v < graph->vertices; v++) {
                    if (!visited[v] && graph->representation.matrix[minVertex][v] != 0) {
                        int newDist = dist[minVertex] + graph->representation.matrix[minVertex][v];
                        if (newDist > 0 && newDist < dist[v]) {
                            dist[v] = newDist;
                        }
                    }
                }
                break;
                
            case ADJ_LIST:
                struct Node *temp = graph->representation.list[minVertex];
                while (temp) {
                    if (!visited[temp->vertex]) {
                        int newDist = dist[minVertex] + temp->weight;
                        if (newDist > 0 && newDist < dist[temp->vertex]) {
                            dist[temp->vertex] = newDist;
                        }
                    }
                    temp = temp->next;
                }
                break;
        }
    }
    
    printf("Shortest distances from vertex %d:\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("Vertex %d: unreachable\n", i);
        } else {
            printf("Vertex %d: %d\n", i, dist[i]);
        }
    }
    
    free(dist);
    free(visited);
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
            for (int i =