//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
} Graph;

void initialize_graph(Graph *g, int vertices) {
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        vertices = MAX_VERTICES;
    }
    g->num_vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i == j) {
                g->adjacency[i][j] = 0;
            } else {
                g->adjacency[i][j] = INF;
            }
        }
    }
}

void add_edge(Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_vertices || v < 0 || v >= g->num_vertices) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

int min_distance(int dist[], int visited[], int n) {
    int min = INF;
    int min_index = -1;
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph *g, int src) {
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    
    for (int i = 0; i < g->num_vertices; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < g->num_vertices - 1; count++) {
        int u = min_distance(dist, visited, g->num_vertices);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < g->num_vertices; v++) {
            if (!visited[v] && g->adjacency[u][v] != INF && dist[u] != INF) {
                long sum = (long)dist[u] + g->adjacency[u][v];
                if (sum < dist[v] && sum <= INT_MAX) {
                    dist[v] = (int)sum;
                }
            }
        }
    }
    
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < g->num_vertices; i++) {
        if (dist[i] == INF) {
            printf("%d \t INF\n", i);
        } else {
            printf("%d \t %d\n", i, dist[i]);
        }
    }
}

int main() {
    Graph g;
    int vertices = 6;
    
    initialize_graph(&g, vertices);
    
    add_edge(&g, 0, 1, 4);
    add_edge(&g, 0, 2, 2);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 3, 5);
    add_edge(&g, 2, 3, 8);
    add_edge(&g, 2, 4, 10);
    add_edge(&g, 3, 4, 2);
    add_edge(&g, 3, 5, 6);
    add_edge(&g, 4, 5, 3);
    
    int source_vertex;
    printf("Enter source vertex (0-%d): ", vertices - 1);
    if (scanf("%d", &source_vertex) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (source_vertex < 0 || source_vertex >= vertices) {
        printf("Source vertex out of range\n");
        return 1;
    }
    
    dijkstra(&g, source_vertex);
    
    return 0;
}