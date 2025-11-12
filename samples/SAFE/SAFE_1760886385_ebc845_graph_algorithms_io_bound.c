//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
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

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int validate_node(int node, int max_nodes) {
    return node >= 0 && node < max_nodes;
}

int validate_edge(struct Edge *edge, int max_nodes) {
    if (!validate_node(edge->src, max_nodes)) return 0;
    if (!validate_node(edge->dest, max_nodes)) return 0;
    if (edge->src == edge->dest) return 0;
    return 1;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) return 0;
    if (!validate_node(src, graph->num_nodes)) return 0;
    if (!validate_node(dest, graph->num_nodes)) return 0;
    if (src == dest) return 0;
    
    struct Edge new_edge;
    new_edge.src = src;
    new_edge.dest = dest;
    new_edge.weight = weight;
    
    if (!validate_edge(&new_edge, graph->num_nodes)) return 0;
    
    graph->edges[graph->num_edges] = new_edge;
    graph->num_edges++;
    return 1;
}

void print_graph(struct Graph *graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    for (int i = 0; i < graph->num_edges; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               graph->edges[i].src, 
               graph->edges[i].dest, 
               graph->edges[i].weight);
    }
}

void bfs(struct Graph *graph, int start_node) {
    if (!validate_node(start_node, graph->num_nodes)) {
        printf("Invalid start node for BFS\n");
        return;
    }
    
    int visited[MAX_NODES];
    for (int i = 0; i < graph->num_nodes; i++) {
        visited[i] = 0;
    }
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start_node] = 1;
    queue[rear++] = start_node;
    
    printf("BFS traversal from node %d: ", start_node);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == current && !visited[graph->edges[i].dest]) {
                visited[graph->edges[i].dest] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = graph->edges[i].dest;
                }
            }
        }
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int num_nodes;
    if (scanf("%d", &num_nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_nodes < 1 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    graph.num_nodes = num_nodes;
    
    printf("Enter number of edges: ");
    int num_edges;
    if (scanf("%d", &num_edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_edges < 0 || num_edges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'source destination weight' (nodes 0-%d):\n", num_nodes - 1);
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        
        if (!add_edge(&graph, src, dest, weight)) {
            printf("Failed to add edge %d->%d\n", src, dest);
            return 1;
        }
    }
    
    print_graph(&graph);
    
    printf("Enter start node for BFS (0-%d): ", num_nodes - 1);
    int start_node;
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    bfs(&graph, start_node);
    
    return 0;
}