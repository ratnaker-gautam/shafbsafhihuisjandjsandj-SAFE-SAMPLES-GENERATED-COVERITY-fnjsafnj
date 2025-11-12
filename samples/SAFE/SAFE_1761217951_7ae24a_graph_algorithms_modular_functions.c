//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

struct Graph {
    int nodes;
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
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            graph->adj[i][j] = 0;
        }
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->nodes || dest < 0 || dest >= graph->nodes) {
        return;
    }
    graph->adj[src][dest] = weight;
    graph->adj[dest][src] = weight;
}

int minDistance(int dist[], int visited[], int nodes) {
    int min = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < nodes; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(struct Graph* graph, int src) {
    if (graph == NULL || src < 0 || src >= graph->nodes) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < graph->nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    for (int count = 0; count < graph->nodes - 1; count++) {
        int u = minDistance(dist, visited, graph->nodes);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->adj[u][v] != 0 && dist[u] != INT_MAX) {
                int new_dist = dist[u] + graph->adj[u][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d \t INF\n", i);
        } else {
            printf("%d \t %d\n", i, dist[i]);
        }
    }
}

void dfsUtil(struct Graph* graph, int v, int visited[]) {
    if (graph == NULL || v < 0 || v >= graph->nodes || visited[v]) {
        return;
    }
    visited[v] = 1;
    printf("%d ", v);
    for (int i = 0; i < graph->nodes; i++) {
        if (graph->adj[v][i] != 0 && !visited[i]) {
            dfsUtil(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from vertex %d: ", start);
    dfsUtil(graph, start, visited);
    printf("\n");
}

int main() {
    struct Graph* graph = createGraph(6);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 2, 3);
    addEdge(graph, 1, 2, 1);
    addEdge(graph, 1, 3, 2);
    addEdge(graph, 2, 3, 4);
    addEdge(graph, 3, 4, 2);
    addEdge(graph, 4, 5, 6);
    dfs(graph, 0);
    dijkstra(graph, 0);
    free(graph);
    return 0;
}