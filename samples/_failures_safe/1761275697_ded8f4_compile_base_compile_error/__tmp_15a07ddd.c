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

int graph_add_node(struct Graph *g, int id, const char *label) {
    if (g == NULL || label == NULL) return 0;
    if (g->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    
    g->nodes[g->node_count].id = id;
    strncpy(g->nodes[g->node_count].label, label, 31);
    g->nodes[g->node_count].label[31] = '\0';
    g->node_count++;
    return 1;
}

int graph_add_edge(struct Graph *g, int from, int to, int weight) {
    if (g == NULL) return 0;
    if (g->edge_count >= MAX_EDGES) return 0;
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == from) from_exists = 1;
        if (g->nodes[i].id == to) to_exists = 1;
    }
    if (!from_exists || !to_exists) return 0;
    
    g->edges[g->edge_count].from = from;
    g->edges[g->edge_count].to = to;
    g->edges[g->edge_count].weight = weight;
    g->edge_count++;
    return 1;
}

void graph_print(struct Graph *g) {
    if (g == NULL) return;
    
    printf("Nodes (%d):\n", g->node_count);
    for (int i = 0; i < g->node_count; i++) {
        printf("  %d: %s\n", g->nodes[i].id, g->nodes[i].label);
    }
    
    printf("Edges (%d):\n", g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("  %d -> %d (weight: %d)\n", 
               g->edges[i].from, g->edges[i].to, g->edges[i].weight);
    }
}

int graph_dijkstra(struct Graph *g, int start, int end, int *distance, int *path) {
    if (g == NULL || distance == NULL || path == NULL) return 0;
    if (start < 0 || start >= MAX_NODES || end < 0 || end >= MAX_NODES) return 0;
    
    int start_exists = 0, end_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start) start_exists = 1;
        if (g->nodes[i].id == end) end_exists = 1;
    }
    if (!start_exists || !end_exists) return 0;
    
    int visited[MAX_NODES] = {0};
    int prev[MAX_NODES];
    
    for (int i = 0; i < MAX_NODES; i++) {
        distance[i] = INT_MAX;
        prev[i] = -1;
    }
    distance[start] = 0;
    
    for (int count = 0; count < g->node_count; count++) {
        int min_dist = INT_MAX;
        int current = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            int node_id = g->nodes[i].id;
            if (!visited[node_id] && distance[node_id] < min_dist) {
                min_dist = distance[node_id];
                current = node_id;
            }
        }
        
        if (current == -1 || current == end) break;
        visited[current] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current) {
                int neighbor = g->edges[i].to;
                int new_dist = distance[current] + g->edges[i].weight;
                if (new_dist > 0 && new_dist < distance[neighbor]) {
                    distance[neighbor] = new_dist;
                    prev[neighbor] = current;
                }
            }
        }
    }
    
    if (distance[end] == INT_MAX) return 0;
    
    int path_len = 0;
    int node = end;
    while (node !=