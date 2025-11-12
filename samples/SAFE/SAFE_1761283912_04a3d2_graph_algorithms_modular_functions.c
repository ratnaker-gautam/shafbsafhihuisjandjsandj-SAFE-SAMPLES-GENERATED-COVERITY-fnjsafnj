//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
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

struct Graph* createGraph(int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->nodes = nodes;
    graph->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
    return graph;
}

int addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->nodes || dest < 0 || dest >= graph->nodes) {
        return 0;
    }
    if (src == dest) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->adj[src][dest] = weight;
    graph->edges++;
    return 1;
}

void printGraph(struct Graph* graph) {
    if (graph == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", graph->nodes, graph->edges);
    for (int i = 0; i < graph->nodes; i++) {
        printf("Node %d: ", i);
        for (int j = 0; j < graph->nodes; j++) {
            if (graph->adj[i][j] != INF && i != j) {
                printf("%d(%d) ", j, graph->adj[i][j]);
            }
        }
        printf("\n");
    }
}

void dijkstra(struct Graph* graph, int src) {
    if (graph == NULL || src < 0 || src >= graph->nodes) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int min = INF;
        int u = -1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->adj[u][v] != INF && dist[u] != INF) {
                long new_dist = (long)dist[u] + graph->adj[u][v];
                if (new_dist < dist[v] && new_dist < INF) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    printf("Shortest distances from node %d:\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph* graph = createGraph(5);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    addEdge(graph, 0, 1, 10);
    addEdge(graph, 0, 2, 5);
    addEdge(graph, 1, 2, 2);
    addEdge(graph, 1, 3, 1);
    addEdge(graph, 2, 1, 3);
    addEdge(graph, 2, 3, 9);
    addEdge(graph, 2, 4, 2);
    addEdge(graph, 3, 4, 4);
    addEdge(graph, 4, 3, 6);
    printGraph(graph);
    printf("\n");
    dijkstra(graph, 0);
    free(graph);
    return 0;
}