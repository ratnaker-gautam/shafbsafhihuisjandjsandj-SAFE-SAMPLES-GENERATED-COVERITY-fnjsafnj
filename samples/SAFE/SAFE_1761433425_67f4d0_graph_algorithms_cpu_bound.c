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

void initGraph(struct Graph* g, int vertices) {
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        vertices = MAX_VERTICES;
    }
    g->vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i == j) {
                g->adjMatrix[i][j] = 0;
            } else {
                g->adjMatrix[i][j] = INF;
            }
        }
    }
}

void addEdge(struct Graph* g, int src, int dest, int weight) {
    if (src < 0 || src >= g->vertices || dest < 0 || dest >= g->vertices) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->adjMatrix[src][dest] = weight;
    g->adjMatrix[dest][src] = weight;
}

void floydWarshall(struct Graph* g) {
    int dist[MAX_VERTICES][MAX_VERTICES];
    for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++) {
            dist[i][j] = g->adjMatrix[i][j];
        }
    }
    for (int k = 0; k < g->vertices; k++) {
        for (int i = 0; i < g->vertices; i++) {
            for (int j = 0; j < g->vertices; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        if (dist[i][k] <= INF - dist[k][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                        }
                    }
                }
            }
        }
    }
    printf("Shortest path distances:\n");
    for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

void dfsUtil(struct Graph* g, int vertex, int visited[]) {
    if (vertex < 0 || vertex >= g->vertices) {
        return;
    }
    visited[vertex] = 1;
    printf("%d ", vertex);
    for (int i = 0; i < g->vertices; i++) {
        if (g->adjMatrix[vertex][i] != INF && !visited[i]) {
            dfsUtil(g, i, visited);
        }
    }
}

void dfs(struct Graph* g, int start) {
    if (start < 0 || start >= g->vertices) {
        return;
    }
    int visited[MAX_VERTICES] = {0};
    printf("DFS traversal from %d: ", start);
    dfsUtil(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph g;
    int vertices = 6;
    initGraph(&g, vertices);
    addEdge(&g, 0, 1, 4);
    addEdge(&g, 0, 2, 3);
    addEdge(&g, 1, 2, 1);
    addEdge(&g, 1, 3, 2);
    addEdge(&g, 2, 3, 4);
    addEdge(&g, 3, 4, 2);
    addEdge(&g, 4, 5, 6);
    floydWarshall(&g);
    dfs(&g, 0);
    return 0;
}