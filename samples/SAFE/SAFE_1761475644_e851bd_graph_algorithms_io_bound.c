//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

void init_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
}

int validate_node(int node, int max_nodes) {
    return node >= 0 && node < max_nodes;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (!validate_node(src, MAX_NODES) || !validate_node(dest, MAX_NODES)) {
        return 0;
    }
    if (src == dest) {
        return 0;
    }
    g->edges[g->num_edges].src = src;
    g->edges[g->num_edges].dest = dest;
    g->edges[g->num_edges].weight = weight;
    g->num_edges++;
    return 1;
}

void print_graph(struct Graph *g) {
    printf("Graph with %d nodes and %d edges:\n", g->num_nodes, g->num_edges);
    for (int i = 0; i < g->num_edges; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               g->edges[i].src, g->edges[i].dest, g->edges[i].weight);
    }
}

void bfs(struct Graph *g, int start) {
    if (!validate_node(start, g->num_nodes)) {
        printf("Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from node %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->num_edges; i++) {
            if (g->edges[i].src == current) {
                int neighbor = g->edges[i].dest;
                if (!visited[neighbor]) {
                    visited[neighbor] = 1;
                    if (rear < MAX_NODES) {
                        queue[rear++] = neighbor;
                    }
                }
            }
        }
    }
    printf("\n");
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
    struct Graph graph;
    init_graph(&graph);
    
    printf("Graph Construction\n");
    printf("==================\n");
    
    graph.num_nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (graph.num_nodes == -1) {
        return 1;
    }
    
    int num_edges = read_int("Enter number of edges: ", 0, MAX_EDGES);
    if (num_edges == -1) {
        return 1;
    }
    
    for (int i = 0; i < num_edges; i++) {
        printf("\nEdge %d:\n", i + 1);
        int src = read_int("  Source node: ", 0, graph.num_nodes - 1);
        if (src == -1) return 1;
        
        int dest = read_int("  Destination node: ", 0, graph.num_nodes - 1);
        if (dest == -1) return 1;
        
        int weight = read_int("  Weight: ", -1000, 1000);
        if (weight == -1) return 1;
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge. Maximum edges reached or invalid nodes.\n");
            i--;
        }
    }
    
    printf("\n");
    print_graph(&graph);
    printf("\n");
    
    int start_node = read_int("Enter start node for BFS: ", 0, graph.num_nodes - 1);
    if (start_node == -1) {
        return 1;
    }
    
    bfs(&graph, start_node);
    
    return 0;
}