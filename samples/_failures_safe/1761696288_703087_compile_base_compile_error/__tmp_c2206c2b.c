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
    
    struct Node *n = &g->nodes[g->node_count];
    n->id = g->node_count;
    strncpy(n->label, label, 31);
    n->label[31] = '\0';
    g->node_count++;
    return n->id;
}

int graph_add_edge(struct Graph *g, int from, int to, int weight) {
    if (g == NULL) return -1;
    if (from < 0 || from >= g->node_count) return -1;
    if (to < 0 || to >= g->node_count) return -1;
    if (g->edge_count >= MAX_EDGES) return -1;
    
    struct Edge *e = &g->edges[g->edge_count];
    e->from = from;
    e->to = to;
    e->weight = weight;
    g->edge_count++;
    return g->edge_count - 1;
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
        
        for (int i = 0; i < g->node_count; i++) {
            if (!result->visited[i] && result->distances[i] < min_dist) {
                min_dist = result->distances[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        result->visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == min_node) {
                int to = g->edges[i].to;
                int weight = g->edges[i].weight;
                if (weight < 0) continue;
                
                int new_dist;
                if (result->distances[min_node] > INT_MAX - weight) {
                    new_dist = INT_MAX;
                } else {
                    new_dist = result->distances[min_node] + weight;
                }
                
                if (!result->visited[to] && new_dist < result->distances[to]) {
                    result->distances[to] = new_dist;
                    result->previous[to] = min_node;
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
        if (path_len