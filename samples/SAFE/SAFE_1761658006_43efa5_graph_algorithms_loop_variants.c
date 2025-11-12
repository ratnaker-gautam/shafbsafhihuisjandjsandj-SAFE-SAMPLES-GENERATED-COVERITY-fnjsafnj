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
        nodes = MAX_NODES;
    }
    g->nodes = nodes;
    g->edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

int addEdge(struct Graph* g, int u, int v, int weight) {
    if (g == NULL || u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    if (g->adj[u][v] == INF) {
        g->edges++;
    }
    g->adj[u][v] = weight;
    return 1;
}

void floydWarshall(struct Graph* g) {
    if (g == NULL || g->nodes <= 0) {
        return;
    }
    
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
                if (dist[k][j] != INF && dist[i][k] != INF) {
                    long long sum = (long long)dist[i][k] + dist[k][j];
                    if (sum > INT_MAX) continue;
                    if (dist[i][j] > sum) {
                        dist[i][j] = (int)sum;
                    }
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

void dfsUtil(struct Graph* g, int node, int visited[]) {
    if (g == NULL || visited == NULL || node < 0 || node >= g->nodes) {
        return;
    }
    visited[node] = 1;
    printf("%d ", node);
    
    for (int i = 0; i < g->nodes; i++) {
        if (g->adj[node][i] != INF && g->adj[node][i] != 0 && !visited[i]) {
            dfsUtil(g, i, visited);
        }
    }
}

void depthFirstSearch(struct Graph* g, int start) {
    if (g == NULL || start < 0 || start >= g->nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    printf("DFS traversal from node %d: ", start);
    dfsUtil(g, start, visited);
    printf("\n");
}

int main() {
    struct Graph graph;
    int nodes = 6;
    
    initializeGraph(&graph, nodes);
    
    addEdge(&graph, 0, 1, 5);
    addEdge(&graph, 0, 2, 3);
    addEdge(&graph, 1, 3, 6);
    addEdge(&graph, 1, 4, 2);
    addEdge(&graph, 2, 1, 1);
    addEdge(&graph, 2, 4, 7);
    addEdge(&graph, 3, 5, 4);
    addEdge(&graph, 4, 3, 1);
    addEdge(&graph, 4, 5, 3);
    
    printf("Graph with %d nodes and %d edges\n", graph.nodes, graph.edges);
    
    floydWarshall(&graph);
    
    depthFirstSearch(&graph, 0);
    
    int counter = 0;
    while (counter < 3) {
        depthFirstSearch(&graph, counter);
        counter++;
    }
    
    for (int i = 3; i < graph.nodes; i++) {
        depthFirstSearch(&graph, i);
    }
    
    return 0;
}