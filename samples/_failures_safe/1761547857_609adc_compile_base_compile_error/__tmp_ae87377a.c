//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000
#define MAX_LINE 1024

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
    int adjacency[MAX_NODES][MAX_NODES];
} Graph;

void initialize_graph(Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) {
        return 0;
    }
    if (g->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->adjacency[from][to] = weight;
    g->edge_count++;
    
    if (from > g->node_count - 1) g->node_count = from + 1;
    if (to > g->node_count - 1) g->node_count = to + 1;
    
    return 1;
}

void floyd_warshall(Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < g->node_count; i++) {
        for (int j = 0; j < g->node_count; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else if (g->adjacency[i][j] != 0) {
                dist[i][j] = g->adjacency[i][j];
            } else {
                dist[i][j] = 1000000;
            }
        }
    }
    
    for (int k = 0; k < g->node_count; k++) {
        for (int i = 0; i < g->node_count; i++) {
            for (int j = 0; j < g->node_count; j++) {
                if (dist[i][k] != 1000000 && dist[k][j] != 1000000) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

int read_graph_from_input(Graph *g) {
    char buffer[MAX_LINE];
    int node_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buffer, "%d", &node_count) != 1) {
        return 0;
    }
    if (node_count < 1 || node_count > MAX_NODES) {
        return 0;
    }
    
    g->node_count = node_count;
    
    printf("Enter edges as 'from to weight' (negative weight to stop):\n");
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        int from, to, weight;
        if (sscanf(buffer, "%d %d %d", &from, &to, &weight) != 3) {
            break;
        }
        
        if (weight < 0) {
            break;
        }
        
        if (!add_edge(g, from, to, weight)) {
            printf("Invalid edge or maximum edges reached\n");
            break;
        }
    }
    
    return 1;
}

void print_shortest_paths(Graph *g) {
    int dist[MAX_NODES][MAX_NODES];
    
    if (g->node_count == 0) {
        printf("Graph is empty\n");
        return;
    }
    
    floyd_warshall(g, dist);
    
    printf("\nShortest path distances:\n");
    printf("    ");
    for (int i = 0; i < g->node_count; i++) {
        printf("%4d", i);
    }
    printf("\n");
    
    for (int i = 0; i < g->node_count; i++) {
        printf("%4d", i);
        for (int j = 0; j < g->node_count; j++) {
            if (dist[i][j] == 1000000) {
                printf("   -");
            } else {
                printf("%4d", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    Graph graph;
    
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n");
    
    if (!read_graph_from_input(&graph)) {
        printf("Error reading graph data\n");
        return 1;
    }