//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void init_graph(struct Graph *g, int nodes, int edges) {
    if (g == NULL || nodes <= 0 || nodes > MAX_NODES || edges < 0 || edges > MAX_EDGES) {
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = edges;
    memset(g->edges, 0, sizeof(struct Edge) * MAX_EDGES);
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g == NULL || src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        return 0;
    }
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    if (g == NULL) {
        return;
    }
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    struct Edge *start = g->edges;
    struct Edge *end = g->edges + g->num_edges;
    while (start < end) {
        printf("  %d -> %d (weight: %d)\n", start->src, start->dest, start->weight);
        start++;
    }
}

int find_shortest_path(struct Graph *g, int start, int end) {
    if (g == NULL || start < 0 || start >= g->num_nodes || end < 0 || end >= g->num_nodes) {
        return -1;
    }
    if (start == end) {
        return 0;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] != -1 && (min_dist == -1 || dist[i] < min_dist)) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        if (min_node == -1) {
            break;
        }
        visited[min_node] = 1;
        
        struct Edge *edge_ptr = g->edges;
        struct Edge *end_ptr = g->edges + g->num_edges;
        while (edge_ptr < end_ptr) {
            if (edge_ptr->src == min_node && !visited[edge_ptr->dest]) {
                int new_dist = dist[min_node] + edge_ptr->weight;
                if (dist[edge_ptr->dest] == -1 || new_dist < dist[edge_ptr->dest]) {
                    dist[edge_ptr->dest] = new_dist;
                }
            }
            edge_ptr++;
        }
    }
    
    return dist[end];
}

int main() {
    struct Graph g;
    int num_nodes, num_edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    init_graph(&g, num_nodes, num_edges);
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, src, dest, weight)) {
            printf("Failed to add edge\n");
            return 1;
        }
    }
    
    print_graph(&g);
    
    int start, end;
    printf("Enter start and end nodes for shortest path: ");
    if (scanf("%d %d", &start, &end) != 2) {
        printf("Invalid node input\n");
        return 1;
    }
    
    int distance = find_shortest_path(&g, start, end);
    if (distance == -1) {
        printf("No path exists between %d and %d\n", start, end);
    } else {
        printf("Shortest