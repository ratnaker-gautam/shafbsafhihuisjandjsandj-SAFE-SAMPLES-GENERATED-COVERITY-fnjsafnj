//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int adj[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(struct Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}

int find_min_distance(int dist[], int visited[], int nodes) {
    int min = INF;
    int min_index = -1;
    for (int v = 0; v < nodes; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* g, int src) {
    if (src < 0 || src >= g->nodes) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < g->nodes - 1; count++) {
        int u = find_min_distance(dist, visited, g->nodes);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < g->nodes; v++) {
            if (visited[v] == 0 && g->adj[u][v] != INF && dist[u] != INF) {
                long new_dist = (long)dist[u] + g->adj[u][v];
                if (new_dist < dist[v] && new_dist <= INT_MAX) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == INF) {
            printf("%d \t\t INF\n", i);
        } else {
            printf("%d \t\t %d\n", i, dist[i]);
        }
    }
}

void floyd_warshall(struct Graph* g) {
    int dist[MAX_NODES][MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }
    for (int k = 0; k < g->nodes; k++) {
        for (int i = 0; i < g->nodes; i++) {
            for (int j = 0; j < g->nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    long new_dist = (long)dist[i][k] + dist[k][j];
                    if (new_dist < dist[i][j] && new_dist <= INT_MAX) {
                        dist[i][j] = (int)new_dist;
                    }
                }
            }
        }
    }
    printf("Floyd-Warshall All Pairs Shortest Path:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    initialize_graph(&g, 5);
    add_edge(&g, 0, 1, 10);
    add_edge(&g, 0, 4, 5);
    add_edge(&g, 1, 2, 1);
    add_edge(&g, 1, 4, 2);
    add_edge(&g, 2, 3, 4);
    add_edge(&g, 3, 2, 6);
    add_edge(&g, 3, 0, 7);
    add_edge(&g, 4, 1, 3);
    add_edge(&g, 4, 2, 9);
    add_edge(&g, 4, 3, 2);
    printf("Dijkstra from vertex 0:\n");
    dijkstra(&g, 0);
    printf("\n");
    floyd_warshall(&g);
    return 0;
}