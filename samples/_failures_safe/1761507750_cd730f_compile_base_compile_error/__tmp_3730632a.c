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
    if (weight < 0 || weight > 1000) return 0;
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    
    if (from >= g->node_count) g->node_count = from + 1;
    if (to >= g->node_count) g->node_count = to + 1;
    
    return 1;
}

void dijkstra(Graph *g, int start, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    for (int i = 0; i < g->node_count; i++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int j = 0; j < g->node_count; j++) {
            if (!visited[j] && distances[j] < min_dist) {
                min_dist = distances[j];
                min_node = j;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int j = 0; j < g->edge_count; j++) {
            if (g->edges[j].from == min_node) {
                int neighbor = g->edges[j].to;
                int new_dist;
                if (distances[min_node] < INT_MAX - g->edges[j].weight) {
                    new_dist = distances[min_node] + g->edges[j].weight;
                } else {
                    new_dist = INT_MAX;
                }
                if (new_dist < distances[neighbor]) {
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
            printf("Input error\n");
            continue;
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
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    int node_count = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    graph.node_count = node_count;
    
    int edge_count = read_int("Enter number of edges: ", 0, MAX_EDGES);
    
    printf("\nEnter edges (from to weight):\n");
    for (int i = 0; i < edge_count; i++) {
        printf("Edge %d: ", i + 1);
        
        int from, to, weight;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            i--;
            continue;
        }
        
        if (sscanf(buffer, "%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid format. Use: from to weight\n");
            i--;
            continue;
        }
        
        if (!add_edge(&graph, from, to, weight)) {
            printf("Invalid edge parameters\n");
            i--;
        }
    }
    
    int start_node = read_int("\nEnter starting node for shortest paths: ", 0, node_count - 1);
    
    int distances[MAX_NODES];
    dijkstra(&graph, start_node, distances);
    
    printf("\nShortest paths from node %d:\n", start_node);
    printf("Node\tDistance\n");
    printf("----\t--------\n");
    
    for (int i = 0; i < graph.node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("%d\tUnreachable\n", i);
        } else {
            printf("%d\t%d\n", i, distances[i]);
        }
    }
    
    printf("\nWould you like to find paths from another node? (1=yes, 0=no): ");
    int choice;