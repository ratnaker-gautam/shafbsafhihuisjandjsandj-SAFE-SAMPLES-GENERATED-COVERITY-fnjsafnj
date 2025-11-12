//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: graph_algorithms
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
    int num_nodes;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

void initialize_graph(struct Graph* graph, int nodes, int edges) {
    if (graph == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        return;
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    for (int i = 0; i < edges; i++) {
        graph->edges[i].src = 0;
        graph->edges[i].dest = 0;
        graph->edges[i].weight = 0;
    }
}

int read_graph(struct Graph* graph) {
    if (graph == NULL) {
        return 0;
    }
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int nodes;
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 0;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    int edges;
    if (scanf("%d", &edges) != 1 || edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 0;
    }
    
    initialize_graph(graph, nodes, edges);
    
    for (int i = 0; i < edges; i++) {
        printf("Enter edge %d (src dest weight): ", i + 1);
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 0;
        }
        if (src < 0 || src >= nodes || dest < 0 || dest >= nodes) {
            printf("Invalid node index\n");
            return 0;
        }
        graph->edges[i].src = src;
        graph->edges[i].dest = dest;
        graph->edges[i].weight = weight;
    }
    
    return 1;
}

void bellman_ford(struct Graph* graph, int source) {
    if (graph == NULL || source < 0 || source >= graph->num_nodes) {
        return;
    }
    
    int dist[MAX_NODES];
    int i = 0;
    while (i < graph->num_nodes) {
        dist[i] = INT_MAX;
        i++;
    }
    dist[source] = 0;
    
    int j;
    for (j = 1; j < graph->num_nodes; j++) {
        int updated = 0;
        int k = 0;
        while (k < graph->num_edges) {
            int u = graph->edges[k].src;
            int v = graph->edges[k].dest;
            int weight = graph->edges[k].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                updated = 1;
            }
            k++;
        }
        if (!updated) {
            break;
        }
    }
    
    int negative_cycle = 0;
    int m = 0;
    do {
        int u = graph->edges[m].src;
        int v = graph->edges[m].dest;
        int weight = graph->edges[m].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            negative_cycle = 1;
            break;
        }
        m++;
    } while (m < graph->num_edges);
    
    if (negative_cycle) {
        printf("Graph contains negative weight cycle\n");
        return;
    }
    
    printf("Shortest distances from node %d:\n", source);
    int n = 0;
    for (n = 0; n < graph->num_nodes; n++) {
        if (dist[n] == INT_MAX) {
            printf("Node %d: unreachable\n", n);
        } else {
            printf("Node %d: %d\n", n, dist[n]);
        }
    }
}

int main() {
    struct Graph graph;
    
    if (!read_graph(&graph)) {
        return 1;
    }
    
    printf("Enter source node: ");
    int source;
    if (scanf("%d", &source) != 1 || source < 0 || source >= graph.num_nodes) {
        printf("Invalid source node\n");
        return 1;
    }
    
    bellman_ford(&graph, source);
    
    return 0;
}