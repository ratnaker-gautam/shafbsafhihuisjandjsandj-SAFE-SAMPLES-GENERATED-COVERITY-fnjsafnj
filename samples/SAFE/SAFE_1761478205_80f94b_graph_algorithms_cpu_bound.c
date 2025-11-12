//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
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

void floydWarshall(struct Graph* graph, int dist[MAX_VERTICES][MAX_VERTICES]) {
    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->vertices; j++) {
            dist[i][j] = graph->adjMatrix[i][j];
        }
    }

    for (int k = 0; k < graph->vertices; k++) {
        for (int i = 0; i < graph->vertices; i++) {
            for (int j = 0; j < graph->vertices; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void printShortestPaths(struct Graph* graph, int dist[MAX_VERTICES][MAX_VERTICES]) {
    printf("Shortest path distances between all pairs:\n");
    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->vertices; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    struct Graph graph;
    int vertices, edges;
    int dist[MAX_VERTICES][MAX_VERTICES];

    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &vertices) != 1 || vertices < 1 || vertices > MAX_VERTICES) {
        printf("Invalid input\n");
        return 1;
    }

    initializeGraph(&graph, vertices);

    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > vertices * (vertices - 1) / 2) {
        printf("Invalid input\n");
        return 1;
    }

    printf("Enter edges as 'source destination weight' (0-indexed):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid input\n");
            return 1;
        }
        if (src < 0 || src >= vertices || dest < 0 || dest >= vertices || weight < 0) {
            printf("Invalid edge parameters\n");
            return 1;
        }
        addEdge(&graph, src, dest, weight);
    }

    floydWarshall(&graph, dist);
    printShortestPaths(&graph, dist);

    return 0;
}