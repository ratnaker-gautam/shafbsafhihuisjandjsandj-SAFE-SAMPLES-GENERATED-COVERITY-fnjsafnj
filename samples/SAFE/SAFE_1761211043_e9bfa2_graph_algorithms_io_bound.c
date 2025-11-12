//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 5000

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

void initialize_graph(Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->node_count = nodes;
    g->edge_count = 0;
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    return 1;
}

void dijkstra(Graph *g, int source, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && distances[v] <= min_dist) {
                min_dist = distances[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == min_node) {
                int neighbor = g->edges[i].to;
                int new_dist;
                if (distances[min_node] < INT_MAX - g->edges[i].weight) {
                    new_dist = distances[min_node] + g->edges[i].weight;
                } else {
                    new_dist = INT_MAX;
                }
                if (!visited[neighbor] && new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                }
            }
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Enter a number between %d and %d\n", min, max);
    }
}

int main() {
    Graph graph;
    int nodes, edges, source;
    int distances[MAX_NODES];
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    initialize_graph(&graph, nodes);
    
    edges = read_int("Enter number of edges: ", 0, MAX_EDGES);
    
    printf("\nEnter edges (from to weight):\n");
    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        printf("Edge %d: ", i + 1);
        
        char buffer[100];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Input error\n");
            return 1;
        }
        
        if (sscanf(buffer, "%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge format. Use: from to weight\n");
            i--;
            continue;
        }
        
        if (!add_edge(&graph, from, to, weight)) {
            printf("Invalid edge. Nodes must be 0-%d, weight non-negative\n", nodes - 1);
            i--;
        }
    }
    
    source = read_int("\nEnter source node (0-%d): ", 0, nodes - 1);
    
    dijkstra(&graph, source, distances);
    
    printf("\nShortest distances from node %d:\n", source);
    printf("Node\tDistance\n");
    printf("----\t--------\n");
    for (int i = 0; i < nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("%d\tUnreachable\n", i);
        } else {
            printf("%d\t%d\n", i, distances[i]);
        }
    }
    
    return 0;
}