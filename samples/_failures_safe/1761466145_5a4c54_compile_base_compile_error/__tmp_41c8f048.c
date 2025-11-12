//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
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

void initGraph(struct Graph* graph, int V, int E) {
    if (V <= 0 || V > MAX_VERTICES || E < 0 || E > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->V = V;
    graph->E = E;
}

int compareEdges(const void* a, const void* b) {
    struct Edge* edge1 = (struct Edge*)a;
    struct Edge* edge2 = (struct Edge*)b;
    return edge1->weight - edge2->weight;
}

int find(int parent[], int i) {
    if (parent[i] == -1) return i;
    return find(parent, parent[i]);
}

void Union(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset != yset) parent[xset] = yset;
}

void kruskalMST(struct Graph* graph) {
    if (graph->V <= 0 || graph->E < 0) {
        fprintf(stderr, "Invalid graph\n");
        return;
    }

    struct Edge result[MAX_VERTICES];
    int e = 0;
    int i = 0;
    
    qsort(graph->edges, graph->E, sizeof(graph->edges[0]), compareEdges);
    
    int parent[MAX_VERTICES];
    for (int v = 0; v < graph->V; v++) parent[v] = -1;
    
    while (e < graph->V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edges[i++];
        
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            Union(parent, x, y);
        }
    }
    
    printf("Minimum Spanning Tree edges:\n");
    int minimumCost = 0;
    for (i = 0; i < e; i++) {
        printf("%d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);
        minimumCost += result[i].weight;
    }
    printf("Total cost: %d\n", minimumCost);
}

void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int src, int V) {
    if (src < 0 || src >= V || V <= 0 || V > MAX_VERTICES) {
        fprintf(stderr, "Invalid parameters for Dijkstra\n");
        return;
    }
    
    int dist[MAX_VERTICES];
    int visited[MAX_VERTICES];
    
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[src] = 0;
    
    for (int count = 0; count < V - 1; count++) {
        int min = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        
        visited[min_index] = 1;
        
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[min_index][v] && dist[min_index] != INT_MAX &&
                dist[min_index] + graph[min_index][v] < dist[v]) {
                dist[v] = dist[min_index] + graph[min_index][v];
            }
        }
    }
    
    printf("Vertex distances from source %d:\n", src);
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX) printf("%d: INF\n", i);
        else printf("%d: %d\n", i, dist[i]);
    }
}

int main() {
    struct Graph graph;
    int V = 5;
    int E = 7;
    
    initGraph(&graph, V, E);
    
    graph.edges[0].src = 0;
    graph.edges[0].dest = 1;
    graph.edges[0].weight = 2;
    
    graph.edges[1].src = 0;
    graph.edges[1].dest = 3;
    graph.edges[1].weight = 6;
    
    graph.edges[2].src = 1;
    graph.edges[2].dest = 2;
    graph.edges[2].weight = 3;
    
    graph.edges[3].src = 1;
    graph.edges[3].dest = 3;
    graph.edges[3].weight = 8;
    
    graph.edges[4].src = 1;
    graph.edges[4].dest = 4;
    graph.edges[4].weight = 5;
    
    graph.edges[5].src = 2;
    graph.edges[5].dest = 4;