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
} Graph;

int read_graph(Graph *graph) {
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &graph->node_count) != 1) {
        return 0;
    }
    if (graph->node_count < 1 || graph->node_count > MAX_NODES) {
        return 0;
    }

    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &graph->edge_count) != 1) {
        return 0;
    }
    if (graph->edge_count < 0 || graph->edge_count > MAX_EDGES) {
        return 0;
    }

    printf("Enter edges as 'from to weight' (nodes 0-%d):\n", graph->node_count - 1);
    for (int i = 0; i < graph->edge_count; i++) {
        if (scanf("%d %d %d", &graph->edges[i].from, &graph->edges[i].to, &graph->edges[i].weight) != 3) {
            return 0;
        }
        if (graph->edges[i].from < 0 || graph->edges[i].from >= graph->node_count ||
            graph->edges[i].to < 0 || graph->edges[i].to >= graph->node_count) {
            return 0;
        }
    }
    return 1;
}

void find_shortest_paths(Graph *graph, int start) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        dist[i] = 1000000;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int count = 0; count < graph->node_count - 1; count++) {
        int min_dist = 1000000;
        int min_node = -1;
        
        for (int i = 0; i < graph->node_count; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == min_node) {
                int to = graph->edges[i].to;
                int new_dist = dist[min_node] + graph->edges[i].weight;
                if (new_dist < dist[to]) {
                    dist[to] = new_dist;
                }
            }
        }
    }

    printf("Shortest paths from node %d:\n", start);
    for (int i = 0; i < graph->node_count; i++) {
        if (dist[i] == 1000000) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, dist[i]);
        }
    }
}

int main() {
    Graph graph;
    
    printf("Graph Shortest Path Finder\n");
    printf("==========================\n");
    
    if (!read_graph(&graph)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    int start_node;
    printf("Enter starting node (0-%d): ", graph.node_count - 1);
    if (scanf("%d", &start_node) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    if (start_node < 0 || start_node >= graph.node_count) {
        printf("Error: Invalid starting node\n");
        return 1;
    }
    
    find_shortest_paths(&graph, start_node);
    
    return 0;
}