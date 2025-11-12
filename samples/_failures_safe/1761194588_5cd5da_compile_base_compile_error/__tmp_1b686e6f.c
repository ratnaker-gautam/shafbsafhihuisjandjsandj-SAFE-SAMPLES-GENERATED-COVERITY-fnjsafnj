//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 5000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        exit(1);
    }
    g->num_nodes = nodes;
    g->num_edges = 0;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            g->adjacency[i][j] = 0;
        }
    }
}

void add_edge(struct Graph *g, int u, int v, int weight) {
    if (u < 0 || u >= g->num_nodes || v < 0 || v >= g->num_nodes) {
        printf("Invalid node indices\n");
        return;
    }
    if (weight < 0) {
        printf("Negative weights not allowed\n");
        return;
    }
    if (g->adjacency[u][v] == 0) {
        g->num_edges++;
    }
    g->adjacency[u][v] = weight;
    g->adjacency[v][u] = weight;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_nodes; i++) {
        printf("Node %d: ", i);
        int first = 1;
        for (int j = 0; j < g->num_nodes; j++) {
            if (g->adjacency[i][j] != 0) {
                if (!first) printf(", ");
                printf("%d(%d)", j, g->adjacency[i][j]);
                first = 0;
            }
        }
        printf("\n");
    }
}

void dijkstra(struct Graph *g, int source) {
    if (source < 0 || source >= g->num_nodes) {
        printf("Invalid source node\n");
        return;
    }
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int v = 0; v < g->num_nodes; v++) {
            if (!visited[v] && g->adjacency[min_node][v] != 0 && 
                dist[min_node] != INT_MAX) {
                int new_dist = dist[min_node] + g->adjacency[min_node][v];
                if (new_dist > 0 && new_dist < dist[v]) {
                    dist[v] = new_dist;
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < g->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    int choice, nodes, u, v, weight, source;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    initialize_graph(&g, nodes);
    
    while (1) {
        printf("\n1. Add edge\n2. Print graph\n3. Find shortest paths\n4. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Enter source, destination, weight: ");
                if (scanf("%d %d %d", &u, &v, &weight) != 3) {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                    break;
                }
                add_edge(&g, u, v, weight);
                break;
            case 2:
                print_graph(&g);
                break;
            case 3:
                printf("Enter source node: ");
                if (scanf("%d", &source) != 1) {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                    break;
                }
                dijkstra(&g, source);
                break;
            case 4:
                return 0;
            default:
                printf("Invalid choice\n");