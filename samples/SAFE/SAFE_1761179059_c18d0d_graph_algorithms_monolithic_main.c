//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int V;
    int E;
    struct Edge edges[MAX_EDGES];
};

void initializeGraph(struct Graph* graph, int V, int E) {
    if (V <= 0 || V > MAX_NODES || E < 0 || E > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->V = V;
    graph->E = E;
}

int validateEdge(int src, int dest, int weight, int V) {
    if (src < 0 || src >= V || dest < 0 || dest >= V) {
        return 0;
    }
    if (weight < -1000 || weight > 1000) {
        return 0;
    }
    return 1;
}

void addEdge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (index < 0 || index >= graph->E) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    if (!validateEdge(src, dest, weight, graph->V)) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    graph->edges[index].src = src;
    graph->edges[index].dest = dest;
    graph->edges[index].weight = weight;
}

void bellmanFord(struct Graph* graph, int src) {
    if (src < 0 || src >= graph->V) {
        fprintf(stderr, "Invalid source node\n");
        return;
    }

    int dist[MAX_NODES];
    for (int i = 0; i < graph->V; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;

    for (int i = 1; i <= graph->V - 1; i++) {
        for (int j = 0; j < graph->E; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    for (int i = 0; i < graph->E; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }

    printf("Vertex distances from source %d:\n", src);
    for (int i = 0; i < graph->V; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d: INFINITY\n", i);
        } else {
            printf("%d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int V, E, src;

    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &V) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &E) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    initializeGraph(&graph, V, E);

    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        addEdge(&graph, i, src, dest, weight);
    }

    printf("Enter source vertex: ");
    if (scanf("%d", &src) != 1) {
        fprintf(stderr, "Invalid source vertex\n");
        return 1;
    }

    bellmanFord(&graph, src);

    return 0;
}