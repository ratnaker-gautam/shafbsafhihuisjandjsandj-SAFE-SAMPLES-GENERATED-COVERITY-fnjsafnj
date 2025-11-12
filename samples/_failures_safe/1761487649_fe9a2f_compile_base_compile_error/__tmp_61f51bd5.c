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

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = INF;
        }
        g->adj[i][i] = 0;
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
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

void floyd_warshall(struct Graph *g) {
    int dist[MAX_NODES][MAX_NODES];
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            dist[i][j] = g->adj[i][j];
        }
    }

    for (int k = 0; k < g->nodes; k++) {
        for (int i = 0; i < g->nodes; i++) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < g->nodes; j++) {
                if (dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    if (dist[i][k] > INF - dist[k][j]) {
                        fprintf(stderr, "Integer overflow detected\n");
                        exit(1);
                    }
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printf("Shortest path distances:\n");
    for (int i = 0; i < g->nodes; i++) {
        for (int j = 0; j < g->nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

void dfs(struct Graph *g, int start) {
    if (start < 0 || start >= g->nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    int stack[MAX_NODES];
    int top = -1;

    stack[++top] = start;
    visited[start] = 1;

    printf("DFS traversal: ");
    while (top >= 0) {
        int current = stack[top--];
        printf("%d ", current);

        for (int i = g->nodes - 1; i >= 0; i--) {
            if (g->adj[current][i] != INF && !visited[i]) {
                visited[i] = 1;
                stack[++top] = i;
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph g;
    int nodes, edges;

    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    init_graph(&g, nodes);

    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    if (edges < 0 || edges > nodes * nodes) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }

    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v, w);
    }

    printf("\nGraph algorithms:\n");
    printf("1. Floyd-Warshall (all pairs shortest path)\n");
    printf("2. DFS traversal\n");

    int choice;
    printf("Enter choice (1 or 2): ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }

    switch (choice) {
        case 1:
            floyd_warshall(&g);
            break;
        case 2:
            printf("Enter start node for DFS: ");
            int start;
            if (scanf("%d", &start) != 1) {
                fprintf(stderr, "Invalid start node\n");
                return 1;
            }
            dfs(&g, start);
            break