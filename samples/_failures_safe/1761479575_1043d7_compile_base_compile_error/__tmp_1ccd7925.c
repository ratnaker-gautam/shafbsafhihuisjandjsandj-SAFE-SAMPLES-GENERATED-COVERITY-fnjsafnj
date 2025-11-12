//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Node {
    int id;
    char label[32];
};

struct Edge {
    int from;
    int to;
    int weight;
};

struct Graph {
    struct Node nodes[MAX_NODES];
    struct Edge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

void graph_init(struct Graph *g) {
    if (g == NULL) return;
    g->node_count = 0;
    g->edge_count = 0;
}

int graph_add_node(struct Graph *g, const char *label) {
    if (g == NULL || label == NULL) return -1;
    if (g->node_count >= MAX_NODES) return -1;
    if (strlen(label) >= 32) return -1;
    
    struct Node *node = &g->nodes[g->node_count];
    node->id = g->node_count;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    g->node_count++;
    return node->id;
}

int graph_add_edge(struct Graph *g, int from, int to, int weight) {
    if (g == NULL) return -1;
    if (from < 0 || from >= g->node_count) return -1;
    if (to < 0 || to >= g->node_count) return -1;
    if (g->edge_count >= MAX_EDGES) return -1;
    
    struct Edge *edge = &g->edges[g->edge_count];
    edge->from = from;
    edge->to = to;
    edge->weight = weight;
    g->edge_count++;
    return 0;
}

void graph_dijkstra(struct Graph *g, int start, int distances[]) {
    if (g == NULL || distances == NULL) return;
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && distances[v] <= min_dist) {
                min_dist = distances[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == min_index) {
                int to = g->edges[i].to;
                int weight = g->edges[i].weight;
                if (!visited[to] && distances[min_index] != INT_MAX && 
                    distances[min_index] + weight < distances[to]) {
                    distances[to] = distances[min_index] + weight;
                }
            }
        }
    }
}

void graph_print_paths(struct Graph *g, int start, int distances[]) {
    if (g == NULL || distances == NULL) return;
    
    printf("Shortest paths from node %d (%s):\n", start, g->nodes[start].label);
    for (int i = 0; i < g->node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("  To %d (%s): No path\n", i, g->nodes[i].label);
        } else {
            printf("  To %d (%s): %d\n", i, g->nodes[i].label, distances[i]);
        }
    }
}

int main() {
    struct Graph g;
    graph_init(&g);
    
    int node_a = graph_add_node(&g, "A");
    int node_b = graph_add_node(&g, "B");
    int node_c = graph_add_node(&g, "C");
    int node_d = graph_add_node(&g, "D");
    int node_e = graph_add_node(&g, "E");
    
    if (node_a < 0 || node_b < 0 || node_c < 0 || node_d < 0 || node_e < 0) {
        fprintf(stderr, "Failed to add nodes\n");
        return 1;
    }
    
    graph_add_edge(&g, node_a, node_b, 4);
    graph_add_edge(&g, node_a, node_c, 2);
    graph_add_edge(&g, node_b, node_c, 1);
    graph_add_edge(&g, node_b, node_d, 5);
    graph_add_edge(&g, node_c, node_d, 8);
    graph_add_edge(&g, node_c, node_e, 10);
    graph_add_edge(&g, node_d, node_e, 2);
    graph_add_edge(&g, node_e, node_d, 3);
    
    int distances[MAX_NODES];
    graph_dijkstra(&g, node_a, distances);
    graph_print_paths(&g, node_a, distances);
    
    printf("\nRunning from node B:\n");
    graph_dijkstra(&g, node_b, distances);
    graph