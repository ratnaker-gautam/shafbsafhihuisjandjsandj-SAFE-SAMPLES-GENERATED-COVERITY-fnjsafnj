//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int nodes;
    int edges[MAX_EDGES][3];
    int edge_count;
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

int add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        printf("Invalid node indices\n");
        return 0;
    }
    if (g->edge_count >= MAX_EDGES) {
        printf("Too many edges\n");
        return 0;
    }
    if (weight < 0) {
        printf("Negative weights not allowed\n");
        return 0;
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
    return 1;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->nodes, g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", 
               i, g->edges[i][0], g->edges[i][1], g->edges[i][2]);
    }
}

void dijkstra(struct Graph *g, int source) {
    if (source < 0 || source >= g->nodes) {
        printf("Invalid source node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    for (int count = 0; count < g->nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        
        for (int i = 0; i < g->nodes; i++) {
            if (!visited[i] && dist[i] != -1 && (min_dist == -1 || dist[i] < min_dist)) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i][0] == min_node) {
                int neighbor = g->edges[i][1];
                int weight = g->edges[i][2];
                int new_dist = dist[min_node] + weight;
                
                if (dist[neighbor] == -1 || new_dist < dist[neighbor]) {
                    dist[neighbor] = new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->nodes; i++) {
        if (dist[i] == -1) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    initialize_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'u v weight' (0-indexed nodes):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (!add_edge(&g, u, v, weight)) {
            return 1;
        }
    }
    
    print_graph(&g);
    
    int source;
    printf("Enter source node for Dijkstra's algorithm: ");
    if (scanf("%d", &source) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    dijkstra(&g, source);
    
    return 0;
}