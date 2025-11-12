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
    g->edge_count++;
    
    g->adjacency[from][to] = weight;
    if (from > g->node_count) g->node_count = from;
    if (to > g->node_count) g->node_count = to;
    
    return 1;
}

void floyd_warshall(Graph *g, int dist[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i <= g->node_count; i++) {
        for (int j = 0; j <= g->node_count; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else if (g->adjacency[i][j] != 0) {
                dist[i][j] = g->adjacency[i][j];
            } else {
                dist[i][j] = 1000000;
            }
        }
    }
    
    for (int k = 0; k <= g->node_count; k++) {
        for (int i = 0; i <= g->node_count; i++) {
            for (int j = 0; j <= g->node_count; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

int read_integer(const char *prompt, int min, int max) {
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
    char buffer[MAX_LINE];
    
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int node_count = read_integer("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (node_count == -1) {
        return 1;
    }
    
    graph.node_count = node_count - 1;
    
    printf("\nEnter edges (from to weight). Enter -1 -1 -1 to finish.\n");
    
    while (1) {
        int from, to, weight;
        
        printf("Edge %d: ", graph.edge_count + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(buffer, "%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid input format. Use: from to weight\n");
            continue;
        }
        
        if (from == -1 && to == -1 && weight == -1) {
            break;
        }
        
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Node indices must be between 0 and %d.\n", node_count - 1);
            continue;
        }
        
        if (weight < 0) {
            printf("Weight must be non-negative.\n");
            continue;
        }
        
        if (!add_edge(&graph, from, to, weight)) {
            printf("Cannot add more edges or invalid parameters.\n");
            break;
        }
    }
    
    if (graph.edge_count == 0) {
        printf("No edges entered. Exiting.\n");
        return 1;
    }
    
    printf("\nComputing shortest paths using Floyd-Warshall algorithm...\n");
    floyd_warshall(&graph, dist);