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

struct Graph* create_graph(int nodes) {
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

int add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->nodes || dest < 0 || dest >= graph->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->adj[src][dest] = weight;
    graph->adj[dest][src] = weight;
    return 1;
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
        int u = find_min_distance(dist, visited, graph->nodes);
        if (u == -1) {
            break;
        }
        visited[u] = 1;
        for (int v = 0; v < graph->nodes; v++) {
            if (!visited[v] && graph->adj[u][v] != 0 && dist[u] != INF) {
                long new_dist = (long)dist[u] + graph->adj[u][v];
                if (new_dist < dist[v] && new_dist <= INF) {
                    dist[v] = (int)new_dist;
                }
            }
        }
    }
    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < graph->nodes; i++) {
        if (dist[i] == INF) {
            printf("%d \t\t INF\n", i);
        } else {
            printf("%d \t\t %d\n", i, dist[i]);
        }
    }
}

void dfs_util(struct Graph* graph, int v, int visited[]) {
    if (graph == NULL || v < 0 || v >= graph->nodes || visited == NULL) {
        return;
    }
    visited[v] = 1;
    printf("%d ", v);
    for (int i = 0; i < graph->nodes; i++) {
        if (graph->adj[v][i] != 0 && !visited[i]) {
            dfs_util(graph, i, visited);
        }
    }
}

void dfs(struct Graph* graph, int start) {
    if (graph == NULL || start < 0 || start >= graph->nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal starting from vertex %d: ", start);
    dfs_util(graph, start, visited);
    printf("\n");
}

int main() {
    struct Graph* graph = create_graph(6);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 3);
    add_edge(graph, 1, 2, 1);
    add_edge(graph, 1, 3, 2);
    add_edge(graph, 2, 3, 4);
    add_edge(graph, 3, 4, 2);
    add_edge(graph, 4, 5, 6);
    dfs(graph, 0);
    dijkstra(graph, 0);
    free(graph);
    return 0;
}