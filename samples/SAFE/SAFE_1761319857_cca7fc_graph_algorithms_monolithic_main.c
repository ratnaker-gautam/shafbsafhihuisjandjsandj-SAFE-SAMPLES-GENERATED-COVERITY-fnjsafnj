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
            fprintf(stderr, "Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Vertex distances from source %d:\n", src);
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX) {
            printf("Vertex %d: INFINITY\n", i);
        } else {
            printf("Vertex %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int V, E;
    
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
    
    if (V <= 0 || V > MAX_NODES || E < 0 || E > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        return 1;
    }
    
    initializeGraph(&graph, V, E);
    
    printf("Enter edges as 'source destination weight':\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        
        if (!validateEdge(src, dest, weight, V)) {
            fprintf(stderr, "Invalid edge parameters\n");
            return 1;
        }
        
        graph.edges[i].src = src;
        graph.edges[i].dest = dest;
        graph.edges[i].weight = weight;
    }
    
    int source;
    printf("Enter source vertex (0-%d): ", V-1);
    if (scanf("%d", &source) != 1) {
        fprintf(stderr, "Invalid source vertex\n");
        return 1;
    }
    
    if (source < 0 || source >= V) {
        fprintf(stderr, "Invalid source vertex\n");
        return 1;
    }
    
    bellmanFord(&graph, source);
    
    return 0;
}