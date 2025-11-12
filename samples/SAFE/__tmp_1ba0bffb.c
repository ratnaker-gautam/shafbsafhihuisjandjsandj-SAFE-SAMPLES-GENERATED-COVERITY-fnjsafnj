//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum GraphType { UNDIRECTED, DIRECTED };

struct Graph {
    int vertices;
    int edges;
    int** adjMatrix;
    enum GraphType type;
};

struct Graph* createGraph(int vertices, enum GraphType type) {
    if (vertices <= 0 || vertices > 100) return NULL;
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph) return NULL;
    graph->vertices = vertices;
    graph->edges = 0;
    graph->type = type;
    graph->adjMatrix = malloc(vertices * sizeof(int*));
    if (!graph->adjMatrix) {
        free(graph);
        return NULL;
    }
    for (int i = 0; i < vertices; i++) {
        graph->adjMatrix[i] = calloc(vertices, sizeof(int));
        if (!graph->adjMatrix[i]) {
            for (int j = 0; j < i; j++) free(graph->adjMatrix[j]);
            free(graph->adjMatrix);
            free(graph);
            return NULL;
        }
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) return;
    if (weight < 0 || weight > 1000) return;
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

void dijkstra(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int* dist = malloc(graph->vertices * sizeof(int));
    int* visited = calloc(graph->vertices, sizeof(int));
    if (!dist || !visited) {
        free(dist);
        free(visited);
        return;
    }
    for (int i = 0; i < graph->vertices; i++) dist[i] = INT_MAX;
    dist[start] = 0;
    for (int count = 0; count < graph->vertices - 1; count++) {
        int minDist = INT_MAX, minIndex = -1;
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && dist[v] <= minDist) {
                minDist = dist[v];
                minIndex = v;
            }
        }
        if (minIndex == -1) break;
        visited[minIndex] = 1;
        for (int v = 0; v < graph->vertices; v++) {
            if (!visited[v] && graph->adjMatrix[minIndex][v] && dist[minIndex] != INT_MAX) {
                long newDist = (long)dist[minIndex] + graph->adjMatrix[minIndex][v];
                if (newDist < dist[v] && newDist <= INT_MAX) dist[v] = newDist;
            }
        }
    }
    printf("Vertex Distance from Source %d\n", start);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INT_MAX) printf("%d \t INF\n", i);
        else printf("%d \t %d\n", i, dist[i]);
    }
    free(dist);
    free(visited);
}

void dfsUtil(struct Graph* graph, int v, int* visited) {
    visited[v] = 1;
    printf("%d ", v);
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->adjMatrix[v][i] && !visited[i]) dfsUtil(graph, i, visited);
    }
}

void dfs(struct Graph* graph, int start) {
    if (!graph || start < 0 || start >= graph->vertices) return;
    int* visited = calloc(graph->vertices, sizeof(int));
    if (!visited) return;
    printf("DFS from vertex %d: ", start);
    dfsUtil(graph, start, visited);
    printf("\n");
    free(visited);
}

void freeGraph(struct Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->vertices; i++) free(graph->adjMatrix[i]);
    free(graph->adjMatrix);
    free(graph);
}

int main() {
    enum GraphType type = UNDIRECTED;
    struct Graph* graph = createGraph(5, type);
    if (!graph) {
        printf("Graph creation failed\n");
        return 1;
    }
    addEdge(graph, 0, 1, 2);
    addEdge(graph, 0, 2, 4);
    addEdge(graph, 1, 2, 1);
    addEdge(graph, 1, 3, 7);
    addEdge(graph, 2, 4, 3);
    addEdge(graph, 3, 4, 1);
    int algorithmChoice;
    printf("Enter algorithm (1 for Dijkstra, 2 for DFS): ");
    if (scanf("%d", &algorithmChoice) != 1) {
        printf("Invalid input\n");
        freeGraph(graph);
        return 1;
    }
    int startVertex;
    printf("Enter start vertex (0-4): ");
    if (scanf("%d", &startVertex) != 1 || startVertex < 0 || startVertex >