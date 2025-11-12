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

void initializeGraph(struct Graph* g, int vertices) {
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        fprintf(stderr, "Invalid number of vertices\n");
        exit(1);
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
        fprintf(stderr, "Invalid vertex index\n");
        exit(1);
    }
    
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    
    g->adjMatrix[src][dest] = weight;
    g->adjMatrix[dest][src] = weight;
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

void dijkstra(struct Graph* g, int src) {
    if (src < 0 || src >= g->vertices) {
        fprintf(stderr, "Invalid source vertex\n");
        return;
    }
    
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    
    for (int i = 0; i < g->vertices; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    
    dist[src] = 0;
    
    for (int count = 0; count < g->vertices - 1; count++) {
        int u = minDistance(dist, visited, g->vertices);
        
        if (u == -1) {
            break;
        }
        
        visited[u] = 1;
        
        for (int v = 0; v < g->vertices; v++) {
            if (!visited[v] && g->adjMatrix[u][v] != INF && dist[u] != INF) {
                long long newDist = (long long)dist[u] + g->adjMatrix[u][v];
                if (newDist < dist[v] && newDist <= INT_MAX) {
                    dist[v] = (int)newDist;
                }
            }
        }
    }
    
    printf("Vertex\tDistance from Source %d\n", src);
    for (int i = 0; i < g->vertices; i++) {
        if (dist[i] == INF) {
            printf("%d\tINF\n", i);
        } else {
            printf("%d\t%d\n", i, dist[i]);
        }
    }
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
                    long long sum = (long long)dist[i][k] + dist[k][j];
                    if (sum < dist[i][j] && sum <= INT_MAX) {
                        dist[i][j] = (int)sum;
                    }
                }
            }
        }
    }
    
    printf("Floyd-Warshall All Pairs Shortest Path:\n");
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