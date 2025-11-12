//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

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

void graph_print(struct Graph *g) {
    if (g == NULL) return;
    
    printf("Nodes (%d):\n", g->node_count);
    for (int i = 0; i < g->node_count; i++) {
        printf("  %d: %s\n", g->nodes[i].id, g->nodes[i].label);
    }
    
    printf("Edges (%d):\n", g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("  %s -> %s (weight: %d)\n", 
               g->nodes[g->edges[i].from].label,
               g->nodes[g->edges[i].to].label,
               g->edges[i].weight);
    }
}

struct DijkstraResult {
    int distances[MAX_NODES];
    int previous[MAX_NODES];
    int visited[MAX_NODES];
};

void dijkstra(struct Graph *g, int start, struct DijkstraResult *result) {
    if (g == NULL || result == NULL) return;
    if (start < 0 || start >= g->node_count) return;
    
    for (int i = 0; i < g->node_count; i++) {
        result->distances[i] = INT_MAX;
        result->previous[i] = -1;
        result->visited[i] = 0;
    }
    result->distances[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!result->visited[v] && result->distances[v] <= min_dist) {
                min_dist = result->distances[v];
                min_node = v;
            }
        }
        
        if (min_node == -1) break;
        result->visited[min_node] = 1;
        
        for (int e = 0; e < g->edge_count; e++) {
            if (g->edges[e].from == min_node) {
                int neighbor = g->edges[e].to;
                int alt;
                if (result->distances[min_node] < INT_MAX - g->edges[e].weight) {
                    alt = result->distances[min_node] + g->edges[e].weight;
                } else {
                    alt = INT_MAX;
                }
                
                if (alt < result->distances[neighbor]) {
                    result->distances[neighbor] = alt;
                    result->previous[neighbor] = min_node;
                }
            }
        }
    }
}

void print_shortest_path(struct Graph *g, struct DijkstraResult *result, int target) {
    if (g == NULL || result == NULL) return;
    if (target < 0 || target >= g->node_count) return;
    
    if (result->distances[target] == INT_MAX) {
        printf("No path to %s\n", g->nodes[target].label);
        return;
    }
    
    printf("Shortest path to %s (distance: %d): ", 
           g->nodes[target].label, result->distances[target]);
    
    int path[MAX_NODES];
    int path_len = 0;
    int current = target;
    
    while (current != -1) {
        path[path_len++] = current;
        current = result->previous[current];
        if (path_len >= MAX_NODES) break;
    }
    
    for (int i = path_len - 1; i >= 0