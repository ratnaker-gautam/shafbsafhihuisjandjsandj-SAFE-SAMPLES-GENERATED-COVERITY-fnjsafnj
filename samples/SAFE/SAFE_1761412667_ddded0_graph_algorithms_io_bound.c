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

void init_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        fprintf(stderr, "Invalid number of nodes\n");
        exit(1);
    }
    g->nodes = nodes;
    g->edge_count = 0;
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (g->edge_count >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    if (u < 0 || u >= g->nodes || v < 0 || v >= g->nodes) {
        fprintf(stderr, "Invalid node index\n");
        exit(1);
    }
    g->edges[g->edge_count][0] = u;
    g->edges[g->edge_count][1] = v;
    g->edges[g->edge_count][2] = weight;
    g->edge_count++;
}

void dijkstra(struct Graph *g, int source, int dist[]) {
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->nodes; i++) {
        dist[i] = -1;
        visited[i] = 0;
    }
    
    if (source < 0 || source >= g->nodes) {
        fprintf(stderr, "Invalid source node\n");
        exit(1);
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
                if (weight < 0) {
                    fprintf(stderr, "Negative weight detected\n");
                    exit(1);
                }
                int new_dist = dist[min_node] + weight;
                if (dist[neighbor] == -1 || new_dist < dist[neighbor]) {
                    dist[neighbor] = new_dist;
                }
            }
        }
    }
}

int main() {
    struct Graph g;
    int nodes, edges;
    int source;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&g, nodes);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (edges < 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            return 1;
        }
        add_edge(&g, u, v, weight);
    }
    
    printf("Enter source node: ");
    if (scanf("%d", &source) != 1) {
        fprintf(stderr, "Invalid source node\n");
        return 1;
    }
    
    int dist[MAX_NODES];
    dijkstra(&g, source, dist);
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g.nodes; i++) {
        if (dist[i] == -1) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
    
    return 0;
}