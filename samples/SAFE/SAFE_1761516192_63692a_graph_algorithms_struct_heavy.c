//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Edge {
    int source;
    int destination;
    int weight;
};

struct Graph {
    int node_count;
    int edge_count;
    struct Edge edges[MAX_EDGES];
};

struct Graph* create_graph(int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return NULL;
    }
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->node_count = nodes;
    graph->edge_count = edges;
    return graph;
}

void destroy_graph(struct Graph* graph) {
    if (graph != NULL) {
        free(graph);
    }
}

int add_edge(struct Graph* graph, int src, int dest, int weight) {
    if (graph == NULL || src < 0 || src >= graph->node_count || 
        dest < 0 || dest >= graph->node_count || 
        graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    graph->edges[graph->edge_count].source = src;
    graph->edges[graph->edge_count].destination = dest;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void bellman_ford(struct Graph* graph, int source) {
    if (graph == NULL || source < 0 || source >= graph->node_count) {
        return;
    }
    
    int distances[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[source] = 0;
    
    for (int i = 1; i <= graph->node_count - 1; i++) {
        for (int j = 0; j < graph->edge_count; j++) {
            int u = graph->edges[j].source;
            int v = graph->edges[j].destination;
            int weight = graph->edges[j].weight;
            
            if (distances[u] != INT_MAX && 
                distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
        }
    }
    
    for (int i = 0; i < graph->edge_count; i++) {
        int u = graph->edges[i].source;
        int v = graph->edges[i].destination;
        int weight = graph->edges[i].weight;
        
        if (distances[u] != INT_MAX && 
            distances[u] + weight < distances[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    
    printf("Vertex distances from source %d:\n", source);
    for (int i = 0; i < graph->node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, distances[i]);
        }
    }
}

int main() {
    int nodes, edges, source;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    struct Graph* graph = create_graph(nodes, 0);
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight':\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            destroy_graph(graph);
            return 1;
        }
        if (!add_edge(graph, src, dest, weight)) {
            printf("Failed to add edge\n");
            destroy_graph(graph);
            return 1;
        }
    }
    
    printf("Enter source node (0-%d): ", nodes - 1);
    if (scanf("%d", &source) != 1 || source < 0 || source >= nodes) {
        printf("Invalid source node\n");
        destroy_graph(graph);
        return 1;
    }
    
    bellman_ford(graph, source);
    destroy_graph(graph);
    
    return 0;
}