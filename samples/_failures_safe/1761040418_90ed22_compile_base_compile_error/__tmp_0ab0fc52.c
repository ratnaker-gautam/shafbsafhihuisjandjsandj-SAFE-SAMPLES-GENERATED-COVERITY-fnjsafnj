//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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
    struct Edge *edges;
};

void init_graph(struct Graph *graph, int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES || edges <= 0 || edges > MAX_EDGES) {
        fprintf(stderr, "Invalid graph parameters\n");
        exit(1);
    }
    graph->num_nodes = nodes;
    graph->num_edges = edges;
    graph->edges = malloc(edges * sizeof(struct Edge));
    if (graph->edges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
}

void free_graph(struct Graph *graph) {
    free(graph->edges);
}

int validate_edge(int src, int dest, int weight, int num_nodes) {
    if (src < 0 || src >= num_nodes || dest < 0 || dest >= num_nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    return 1;
}

void add_edge(struct Graph *graph, int idx, int src, int dest, int weight) {
    if (idx < 0 || idx >= graph->num_edges) {
        fprintf(stderr, "Invalid edge index\n");
        exit(1);
    }
    if (!validate_edge(src, dest, weight, graph->num_nodes)) {
        fprintf(stderr, "Invalid edge parameters\n");
        exit(1);
    }
    (graph->edges + idx)->src = src;
    (graph->edges + idx)->dest = dest;
    (graph->edges + idx)->weight = weight;
}

void print_graph(struct Graph *graph) {
    printf("Graph with %d nodes and %d edges:\n", graph->num_nodes, graph->num_edges);
    for (int i = 0; i < graph->num_edges; i++) {
        printf("Edge %d: %d -> %d (weight %d)\n", i, (graph->edges + i)->src, 
               (graph->edges + i)->dest, (graph->edges + i)->weight);
    }
}

void dijkstra(struct Graph *graph, int start) {
    if (start < 0 || start >= graph->num_nodes) {
        fprintf(stderr, "Invalid start node\n");
        return;
    }

    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        
        for (int i = 0; i < graph->num_nodes; i++) {
            if (!visited[i] && dist[i] != -1 && (min_dist == -1 || dist[i] < min_dist)) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;

        for (int i = 0; i < graph->num_edges; i++) {
            struct Edge *edge = graph->edges + i;
            if (edge->src == min_node && !visited[edge->dest]) {
                int new_dist = dist[min_node] + edge->weight;
                if (dist[edge->dest] == -1 || new_dist < dist[edge->dest]) {
                    dist[edge->dest] = new_dist;
                }
            }
        }
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == -1) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Enter number of edges (1-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    init_graph(&graph, nodes, edges);
    
    printf("Enter edges as: source destination weight\n");
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        printf("Edge %d: ", i);
        if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
            fprintf(stderr, "Invalid edge input\n");
            free_graph(&graph);
            return 1;
        }
        add_edge(&graph, i, src, dest, weight);
    }
    
    print_graph(&graph);
    
    int start_node;
    printf("Enter start node for Dijkstra: ");