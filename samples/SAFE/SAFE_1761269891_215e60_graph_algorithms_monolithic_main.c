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
    graph->V = V;
    graph->E = E;
    for (int i = 0; i < E; i++) {
        graph->edges[i].src = 0;
        graph->edges[i].dest = 0;
        graph->edges[i].weight = 0;
    }
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

int addEdge(struct Graph* graph, int index, int src, int dest, int weight) {
    if (index < 0 || index >= graph->E) {
        return 0;
    }
    if (!validateEdge(src, dest, weight, graph->V)) {
        return 0;
    }
    graph->edges[index].src = src;
    graph->edges[index].dest = dest;
    graph->edges[index].weight = weight;
    return 1;
}

void bellmanFord(struct Graph* graph, int src) {
    int V = graph->V;
    int E = graph->E;
    int dist[MAX_NODES];
    
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;
    
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }
    
    for (int i = 0; i < E; i++) {
        int u = graph->edges[i].src;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Vertex distances from source %d:\n", src);
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d: INFINITY\n", i);
        } else {
            printf("%d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int V, E;
    
    printf("Enter number of vertices (1-100): ");
    if (scanf("%d", &V) != 1 || V < 1 || V > MAX_NODES) {
        printf("Invalid number of vertices\n");
        return 1;
    }
    
    printf("Enter number of edges (0-1000): ");
    if (scanf("%d", &E) != 1 || E < 0 || E > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    initializeGraph(&graph, V, E);
    
    printf("Enter edges as source destination weight:\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!addEdge(&graph, i, src, dest, weight)) {
            printf("Invalid edge parameters\n");
            return 1;
        }
    }
    
    int source;
    printf("Enter source vertex (0-%d): ", V-1);
    if (scanf("%d", &source) != 1 || source < 0 || source >= V) {
        printf("Invalid source vertex\n");
        return 1;
    }
    
    bellmanFord(&graph, source);
    
    return 0;
}