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

void initialize_graph(Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int add_edge(Graph *g, int from, int to, int weight) {
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES) return 0;
    if (to < 0 || to >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    return 1;
}

void dijkstra(Graph *g, int source, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < MAX_NODES; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int count = 0; count < g->node_count; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && distances[i] < min_dist) {
                min_dist = distances[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == min_node) {
                int to = g->edges[i].to;
                int new_dist = distances[min_node] + g->edges[i].weight;
                if (new_dist > 0 && new_dist < distances[to]) {
                    distances[to] = new_dist;
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
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    }
}

int main() {
    Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    graph.node_count = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (graph.node_count == -1) return 1;
    
    int edge_count = read_int("Enter number of edges: ", 0, MAX_EDGES);
    if (edge_count == -1) return 1;
    
    printf("\nEnter edges (from to weight):\n");
    for (int i = 0; i < edge_count; i++) {
        printf("Edge %d: ", i + 1);
        
        int from, to, weight;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input.\n");
            return 1;
        }
        
        if (sscanf(buffer, "%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge format. Please enter: from to weight\n");
            i--;
            continue;
        }
        
        if (!add_edge(&graph, from, to, weight)) {
            printf("Invalid edge parameters. Nodes must be 0-%d, weight non-negative.\n", graph.node_count - 1);
            i--;
        }
    }
    
    int source = read_int("\nEnter source node for shortest paths: ", 0, graph.node_count - 1);
    if (source == -1) return 1;
    
    int distances[MAX_NODES];
    dijkstra(&graph, source, distances);
    
    printf("\nShortest paths from node %d:\n", source);
    printf("Node\tDistance\n");
    printf("----\t--------\n");
    
    for (int i = 0; i < graph.node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("%d\tUnreachable\n", i);
        } else {
            printf("%d\t%d\n", i, distances[i]);
        }
    }
    
    return 0;
}