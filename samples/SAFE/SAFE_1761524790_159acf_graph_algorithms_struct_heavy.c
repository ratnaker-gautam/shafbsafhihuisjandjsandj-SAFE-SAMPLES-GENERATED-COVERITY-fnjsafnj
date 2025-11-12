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

void graph_dijkstra(struct Graph *g, int start, int *distances) {
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

void print_shortest_paths(struct Graph *g, int start) {
    if (g == NULL) return;
    if (start < 0 || start >= g->node_count) return;
    
    int distances[MAX_NODES];
    graph_dijkstra(g, start, distances);
    
    printf("Shortest paths from node %d (%s):\n", start, g->nodes[start].label);
    for (int i = 0; i < g->node_count; i++) {
        if (distances[i] == INT_MAX) {
            printf("  To node %d (%s): No path\n", i, g->nodes[i].label);
        } else {
            printf("  To node %d (%s): Distance %d\n", i, g->nodes[i].label, distances[i]);
        }
    }
}

int main(void) {
    struct Graph g;
    graph_init(&g);
    
    graph_add_node(&g, "A");
    graph_add_node(&g, "B");
    graph_add_node(&g, "C");
    graph_add_node(&g, "D");
    graph_add_node(&g, "E");
    
    graph_add_edge(&g, 0, 1, 4);
    graph_add_edge(&g, 0, 2, 2);
    graph_add_edge(&g, 1, 2, 3);
    graph_add_edge(&g, 1, 3, 2);
    graph_add_edge(&g, 1, 4, 3);
    graph_add_edge(&g, 2, 1, 1);
    graph_add_edge(&g, 2, 3, 4);
    graph_add_edge(&g, 2, 4, 5);
    graph_add_edge(&g, 3, 4, 1);
    
    printf("Graph created with %d nodes and %d edges\n", g.node_count, g.edge_count);
    printf("\n");
    
    print_shortest_paths(&g, 0);
    printf("\n");
    
    print_shortest_paths(&g, 3);
    
    return 0;
}