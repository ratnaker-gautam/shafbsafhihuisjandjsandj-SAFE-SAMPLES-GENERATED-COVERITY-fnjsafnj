//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct Graph {
    int nodes;
    int edges;
    int adj[MAX_NODES][MAX_NODES];
};

void initializeGraph(struct Graph* g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void addEdge(struct Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    if (weight < 0) {
        fprintf(stderr, "Negative weights not allowed\n");
        exit(1);
    }
    g->adj[u][v] = weight;
    g->edges++;
}

void floydWarshall(struct Graph* g) {
    int dist[MAX_NODES][MAX_NODES];
    int i = 0;
    while (i < g->nodes) {
        int j = 0;
        do {
            dist[i][j] = g->adj[i][j];
            j++;
        } while (j < g->nodes);
        i++;
    }

    int k = 0;
    for (; k < g->nodes; k++) {
        int i = 0;
        while (i < g->nodes) {
            int j = 0;
            do {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    long sum = (long)dist[i][k] + dist[k][j];
                    if (sum < dist[i][j] && sum <= INT_MAX) {
                        dist[i][j] = (int)sum;
                    }
                }
                j++;
            } while (j < g->nodes);
            i++;
        }
    }

    printf("Shortest path distances:\n");
    int i2 = 0;
    for (; i2 < g->nodes; i2++) {
        int j = 0;
        while (j < g->nodes) {
            if (dist[i2][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i2][j]);
            }
            j++;
        }
        printf("\n");
    }
}

int main() {
    struct Graph g;
    int nodes = 4;
    initializeGraph(&g, nodes);

    addEdge(&g, 0, 1, 5);
    addEdge(&g, 0, 3, 10);
    addEdge(&g, 1, 2, 3);
    addEdge(&g, 2, 3, 1);

    floydWarshall(&g);

    return 0;
}