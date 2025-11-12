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
    struct Edge edges[MAX_EDGES];
};

void init_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
}

int add_edge(struct Graph *g, int src, int dest, int weight) {
    if (g->num_edges >= MAX_EDGES) return 0;
    if (src < 0 || dest < 0 || src >= MAX_NODES || dest >= MAX_NODES) return 0;
    
    struct Edge *e = g->edges + g->num_edges;
    e->src = src;
    e->dest = dest;
    e->weight = weight;
    g->num_edges++;
    
    if (src >= g->num_nodes) g->num_nodes = src + 1;
    if (dest >= g->num_nodes) g->num_nodes = dest + 1;
    
    return 1;
}

void bellman_ford(struct Graph *g, int start, int *dist) {
    for (int i = 0; i < g->num_nodes; i++) {
        *(dist + i) = 1000000;
    }
    *(dist + start) = 0;
    
    for (int i = 0; i < g->num_nodes - 1; i++) {
        for (int j = 0; j < g->num_edges; j++) {
            struct Edge *e = g->edges + j;
            if (*(dist + e->src) != 1000000 && *(dist + e->src) + e->weight < *(dist + e->dest)) {
                *(dist + e->dest) = *(dist + e->src) + e->weight;
            }
        }
    }
    
    for (int j = 0; j < g->num_edges; j++) {
        struct Edge *e = g->edges + j;
        if (*(dist + e->src) != 1000000 && *(dist + e->src) + e->weight < *(dist + e->dest)) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
}

void print_distances(int *dist, int num_nodes, int start) {
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < num_nodes; i++) {
        if (*(dist + i) == 1000000) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, *(dist + i));
        }
    }
}

int main() {
    struct Graph g;
    int distances[MAX_NODES];
    int choice, src, dest, weight, start;
    
    init_graph(&g);
    
    printf("Graph Shortest Path (Bellman-Ford)\n");
    printf("1. Add edge\n");
    printf("2. Find shortest paths\n");
    printf("3. Exit\n");
    
    while (1) {
        printf("\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Enter source, destination, weight: ");
                if (scanf("%d %d %d", &src, &dest, &weight) != 3) {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                    break;
                }
                if (add_edge(&g, src, dest, weight)) {
                    printf("Edge added successfully\n");
                } else {
                    printf("Failed to add edge\n");
                }
                break;
                
            case 2:
                printf("Enter starting node: ");
                if (scanf("%d", &start) != 1) {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                    break;
                }
                if (start < 0 || start >= g.num_nodes) {
                    printf("Invalid starting node\n");
                    break;
                }
                bellman_ford(&g, start, distances);
                print_distances(distances, g.num_nodes, start);
                break;
                
            case 3:
                return 0;
                
            default:
                printf("Invalid choice\n");
                break;
        }
    }
    
    return 0;
}