//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void initialize_graph(struct Graph *graph) {
    graph->num_nodes = 0;
    graph->num_edges = 0;
}

int add_node(struct Graph *graph) {
    if (graph->num_nodes >= MAX_NODES) {
        return -1;
    }
    return graph->num_nodes++;
}

int add_edge(struct Graph *graph, int src, int dest, int weight) {
    if (graph->num_edges >= MAX_EDGES) {
        return 0;
    }
    if (src < 0 || src >= graph->num_nodes || dest < 0 || dest >= graph->num_nodes) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    graph->edges[graph->num_edges].src = src;
    graph->edges[graph->num_edges].dest = dest;
    graph->edges[graph->num_edges].weight = weight;
    graph->num_edges++;
    return 1;
}

void dijkstra(struct Graph *graph, int start_node) {
    if (start_node < 0 || start_node >= graph->num_nodes) {
        printf("Invalid start node\n");
        return;
    }

    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_node] = 0;

    for (int count = 0; count < graph->num_nodes - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;

        for (int v = 0; v < graph->num_nodes; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_node = v;
            }
        }

        if (min_node == -1) {
            break;
        }

        visited[min_node] = 1;

        for (int i = 0; i < graph->num_edges; i++) {
            if (graph->edges[i].src == min_node) {
                int neighbor = graph->edges[i].dest;
                int weight = graph->edges[i].weight;
                if (!visited[neighbor] && dist[min_node] != INT_MAX) {
                    int new_dist = dist[min_node] + weight;
                    if (new_dist < dist[neighbor]) {
                        dist[neighbor] = new_dist;
                    }
                }
            }
        }
    }

    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < graph->num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);

    printf("Graph Shortest Path Finder\n");
    printf("Creating nodes...\n");

    for (int i = 0; i < 6; i++) {
        add_node(&graph);
    }

    printf("Adding edges...\n");
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);

    printf("Graph has %d nodes and %d edges\n", graph.num_nodes, graph.num_edges);

    int start_node;
    printf("Enter starting node (0-%d): ", graph.num_nodes - 1);
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (start_node < 0 || start_node >= graph.num_nodes) {
        printf("Invalid node number\n");
        return 1;
    }

    dijkstra(&graph, start_node);

    printf("Would you like to find paths from another node? (1=yes, 0=no): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        return 1;
    }

    if (choice == 1) {
        printf("Enter starting node (0-%d): ", graph.num_nodes - 1);
        if (scanf("%d", &start_node) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        if (start_node < 0 || start_node >= graph.num_nodes) {
            printf("Invalid node number\n");
            return 1;
        }
        dijkstra(&graph, start_node);
    }

    return 0;
}