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
    
    if (from > g->node_count - 1) g->node_count = from + 1;
    if (to > g->node_count - 1) g->node_count = to + 1;
    
    g->edge_count++;
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

int read_int(const char *prompt, int min, int max) {
    char buffer[MAX_LINE];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

int main() {
    Graph graph;
    int dist[MAX_NODES][MAX_NODES];
    
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int node_count = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (node_count == -1) {
        return 1;
    }
    
    graph.node_count = node_count;
    
    printf("\nEnter edges (from to weight). Enter -1 -1 -1 to finish.\n");
    
    while (1) {
        int from = read_int("From node: ", -1, node_count - 1);
        if (from == -1) break;
        
        int to = read_int("To node: ", 0, node_count - 1);
        if (to == -1) break;
        
        int weight = read_int("Weight: ", 0, 10000);
        if (weight == -1) break;
        
        if (from == -1 && to == -1 && weight == -1) {
            break;
        }
        
        if (!add_edge(&graph, from, to, weight)) {
            printf("Failed to add edge. Maximum edges reached or invalid parameters.\n");
        }
    }
    
    printf("\nComputing shortest paths using Floyd-Warshall algorithm...\n");
    floyd_warshall(&graph, dist);
    
    printf("\nShortest path distances:\n");
    printf("    ");
    for (int i = 0; i < graph.node_count; i++) {
        printf("%4d", i);
    }
    printf("\n");
    
    for (int i = 0; i < graph.node_count; i++) {
        printf("%2d: ", i);
        for (int j = 0;