//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

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

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            exit(1);
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer between %d and %d\n", min_val, max_val);
    }
}

void initialize_graph(Graph* graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
}

int add_edge(Graph* graph, int from, int to, int weight) {
    if (graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) {
        return 0;
    }
    if (weight < 0 || weight > 1000) {
        return 0;
    }
    
    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(Graph* graph, int start_node) {
    if (start_node < 0 || start_node >= graph->node_count) {
        printf("Invalid start node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_node] = 0;
    
    for (int count = 0; count < graph->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int i = 0; i < graph->node_count; i++) {
            if (!visited[i] && dist[i] <= min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == min_node) {
                int to = graph->edges[i].to;
                int weight = graph->edges[i].weight;
                
                if (!visited[to] && dist[min_node] != INT_MAX) {
                    int new_dist = dist[min_node] + weight;
                    if (new_dist > 0 && new_dist < dist[to]) {
                        dist[to] = new_dist;
                    }
                }
            }
        }
    }
    
    printf("\nShortest distances from node %d:\n", start_node);
    for (int i = 0; i < graph->node_count; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Shortest Path Calculator\n");
    printf("==============================\n\n");
    
    graph.node_count = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    
    int edge_count = read_int("Enter number of edges: ", 0, MAX_EDGES);
    
    printf("\nEnter edges (from to weight):\n");
    for (int i = 0; i < edge_count; i++) {
        printf("Edge %d: ", i + 1);
        
        int from, to, weight;
        char buffer[100];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            return 1;
        }
        
        if (sscanf(buffer, "%d %d %d", &from, &to, &weight) != 3) {
            printf("Invalid edge format. Please enter: from to weight\n");
            i--;
            continue;
        }
        
        if (!add_edge(&graph, from, to, weight)) {
            printf("Invalid edge parameters. Nodes must be 0-%d, weight 0-1000\n", graph.node_count - 1);
            i--;
        }
    }
    
    int start_node = read_int("\nEnter start node for shortest path: ", 0, graph.node_count - 1);
    
    dijkstra(&graph, start_node);
    
    return 0;
}