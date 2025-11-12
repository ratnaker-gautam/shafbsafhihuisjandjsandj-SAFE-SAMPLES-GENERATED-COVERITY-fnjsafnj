//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000
#define MAX_LINE 256

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
    char line[MAX_LINE];
    int node_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }
    if (sscanf(line, "%d", &node_count) != 1) {
        return 0;
    }
    if (node_count < 1 || node_count > MAX_NODES) {
        return 0;
    }
    
    g->node_count = node_count;
    
    printf("Enter edges (format: from to weight), one per line. Enter empty line to finish.\n");
    printf("Nodes are 0-based. Maximum %d edges.\n", MAX_EDGES);
    
    while (g->edge_count < MAX_EDGES) {
        printf("Edge %d: ", g->edge_count + 1);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        if (line[0] == '\n') {
            break;
        }
        
        int from, to, weight;
        if (sscanf(line, "%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid format. Use: from to weight\n");
            continue;
        }
        
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Node indices must be between 0 and %d\n", node_count - 1);
            continue;
        }
        
        if (weight < 0) {
            printf("Weight must be non-negative\n");
            continue;
        }
        
        if (!add_edge(g, from, to, weight)) {
            printf("Failed to add edge\n");
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
    
    for (int i = 0; i < g->node_count; i++)