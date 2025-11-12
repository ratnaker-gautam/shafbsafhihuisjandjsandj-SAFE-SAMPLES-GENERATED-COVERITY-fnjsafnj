//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

struct Graph {
    int vertices;
    int adjMatrix[MAX_VERTICES][MAX_VERTICES];
};

void initializeGraph(struct Graph* graph, int vertices) {
    graph->vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i == j) {
                graph->adjMatrix[i][j] = 0;
            } else {
                graph->adjMatrix[i][j] = INF;
            }
        }
    }
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) {
        return;
    }
    graph->adjMatrix[src][dest] = weight;
    graph->adjMatrix[dest][src] = weight;
}

int minDistance(int dist[], int visited[], int vertices) {
    int min = INF;
    int minIndex = -1;
    for (int v = 0; v < vertices; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void dijkstra(struct Graph* graph, int src) {
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertices; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < graph->vertices - 1; count++) {
        int u = minDistance(dist, visited, graph->vertices);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < graph->vertices; v++) {
            if (visited[v] == 0 && graph->adjMatrix[u][v] != INF && dist[u] != INF) {
                long long newDist = (long long)dist[u] + graph->adjMatrix[u][v];
                if (newDist < dist[v] && newDist <= INT_MAX) {
                    dist[v] = (int)newDist;
                }
            }
        }
    }
    printf("Vertex Distance from Source %d:\n", src);
    for (int i = 0; i < graph->vertices; i++) {
        if (dist[i] == INF) {
            printf("%d: INF\n", i);
        } else {
            printf("%d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int vertices = 6;
    initializeGraph(&graph, vertices);
    addEdge(&graph, 0, 1, 4);
    addEdge(&graph, 0, 2, 2);
    addEdge(&graph, 1, 2, 1);
    addEdge(&graph, 1, 3, 5);
    addEdge(&graph, 2, 3, 8);
    addEdge(&graph, 2, 4, 10);
    addEdge(&graph, 3, 4, 2);
    addEdge(&graph, 3, 5, 6);
    addEdge(&graph, 4, 5, 3);
    printf("Graph with %d vertices created.\n", vertices);
    printf("Running Dijkstra's algorithm from vertex 0:\n");
    dijkstra(&graph, 0);
    return 0;
}