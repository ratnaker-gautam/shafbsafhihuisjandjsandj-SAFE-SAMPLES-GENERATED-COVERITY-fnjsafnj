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

int compareEdges(const void* a, const void* b) {
    struct Edge* edgeA = (struct Edge*)a;
    struct Edge* edgeB = (struct Edge*)b;
    if (edgeA->weight < edgeB->weight) return -1;
    if (edgeA->weight > edgeB->weight) return 1;
    return 0;
}

int find(int parent[], int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void unionSets(int parent[], int rank[], int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    
    if (rank[xroot] < rank[yroot]) {
        parent[xroot] = yroot;
    } else if (rank[xroot] > rank[yroot]) {
        parent[yroot] = xroot;
    } else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

void kruskalMST(struct Graph* graph) {
    int V = graph->V;
    struct Edge result[V];
    int e = 0;
    int i = 0;
    
    qsort(graph->edges, graph->E, sizeof(graph->edges[0]), compareEdges);
    
    int parent[V];
    int rank[V];
    
    for (int v = 0; v < V; v++) {
        parent[v] = v;
        rank[v] = 0;
    }
    
    while (e < V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edges[i++];
        
        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);
        
        if (x != y) {
            result[e++] = next_edge;
            unionSets(parent, rank, x, y);
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

void dijkstra(int graph[MAX_NODES][MAX_NODES], int src, int V) {
    int dist[V];
    int visited[V];
    
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
        if (dist[i] == INT_MAX) {
            printf("%d: INFINITE\n", i);
        } else {
            printf("%d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    int V = 5;
    int E = 7;
    
    struct Graph graph;
    initializeGraph(&graph, V, E);
    
    struct Edge edges[] = {
        {0, 1, 2}, {0, 3, 6}, {1, 2, 3}, {1, 3, 8},
        {1, 4, 5}, {2, 4, 7}, {3, 4, 9}
    };
    
    for (int i = 0; i < E; i++) {
        if (edges[i].src < 0 || edges[i].src >= V || 
            edges[i].dest < 0 || edges[i].dest >= V) {
            fprintf(stderr, "Invalid edge\n");
            return 1;
        }
        graph.edges[i] = edges[i];
    }
    
    printf("Kruskal's MST Algorithm:\n");
    k