//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { DIRECTED, UNDIRECTED };

struct Graph {
    int vertices;
    int edges;
    int **adjMatrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) {
        return NULL;
    }
    
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    
    graph->vertices = vertices;
    graph->edges = 0;
    graph->type = type;
    
    graph->adjMatrix = malloc(vertices * sizeof(int *));
    if (!graph->adjMatrix) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->adjMatrix[i] = calloc(vertices, sizeof(int));
        if (!graph->adjMatrix[i]) {
            for (int j = 0; j < i; j++) {
                free(graph->adjMatrix[j]);
            }
            free(graph->adjMatrix);
            free(graph);
            return NULL;
        }
    }
    
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) {
        return;
    }
    
    if (weight < 0 || weight > 1000) {
        return;
    }
    
    graph->adjMatrix[src][dest] = weight;
    graph->edges++;
    
    switch (graph->type) {
        case UNDIRECTED:
            graph->adjMatrix[dest][src] = weight;
            break;
        case DIRECTED:
            break;
    }
}

void dijkstra(struct Graph *graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) {
        return;
    }
    
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
        int minIndex = -1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minIndex = v;
            }
        }
        
        if (minIndex == -1) break;
        
        visited[minIndex] = 1;
        
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjMatrix[minIndex][v] && 
                dist[minIndex] != INT_MAX && 
                dist[minIndex] + graph->adjMatrix[minIndex][v] < dist[v]) {
                dist[v] = dist[minIndex] + graph->adjMatrix[minIndex][v];
            }
        }
    }
    
    printf("Vertex\tDistance from %d\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d\tINFINITY\n", i);
        } else {
            printf("%d\t%d\n", i, dist[i]);
        }
    }
    
    free(dist);
    free(visited);
}

void freeGraph(struct Graph *graph) {
    if (!graph) return;
    
    for (int i = 0; i < graph->vertices; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);
}

int main() {
    enum GraphType type;
    int choice;
    
    printf("Enter graph type (0 for DIRECTED, 1 for UNDIRECTED): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 0:
            type = DIRECTED;
            break;
        case 1:
            type = UNDIRECTED;
            break;
        default:
            printf("Invalid graph type\n");
            return 1;
    }
    
    int vertices;
    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &vertices) != 1 || vertices < 1 || vertices > 100) {
        printf("Invalid number of vertices\n");
        return 1;
    }
    
    struct Graph *graph = createGraph(vertices, type);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    int edges;
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > vertices * vertices) {
        printf("Invalid number of edges\n");
        freeGraph(graph);
        return 1;
    }
    
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Enter edge %d (src dest weight): ", i + 1);
        if (scanf("%d %d %d", &src, &dest, &weight)