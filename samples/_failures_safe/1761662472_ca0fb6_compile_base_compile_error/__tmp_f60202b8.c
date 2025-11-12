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

struct Graph create_graph(void) {
    struct Graph g;
    g.node_count = 0;
    g.edge_count = 0;
    return g;
}

int add_node(struct Graph *g, int id, const char *label) {
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

int add_edge(struct Graph *g, int from, int to, int weight) {
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

void print_graph(const struct Graph *g) {
    if (g == NULL) return;
    printf("Graph nodes (%d):\n", g->node_count);
    for (int i = 0; i < g->node_count; i++) {
        printf("  Node %d: %s\n", g->nodes[i].id, g->nodes[i].label);
    }
    printf("Graph edges (%d):\n", g->edge_count);
    for (int i = 0; i < g->edge_count; i++) {
        printf("  Edge %d -> %d (weight %d)\n", 
               g->edges[i].from, g->edges[i].to, g->edges[i].weight);
    }
}

struct DijkstraResult {
    int distances[MAX_NODES];
    int previous[MAX_NODES];
    int valid[MAX_NODES];
};

struct DijkstraResult dijkstra(const struct Graph *g, int start) {
    struct DijkstraResult result;
    if (g == NULL || start < 0 || start >= MAX_NODES) {
        for (int i = 0; i < MAX_NODES; i++) {
            result.distances[i] = INT_MAX;
            result.previous[i] = -1;
            result.valid[i] = 0;
        }
        return result;
    }
    int start_exists = 0;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == start) {
            start_exists = 1;
            break;
        }
    }
    if (!start_exists) {
        for (int i = 0; i < MAX_NODES; i++) {
            result.distances[i] = INT_MAX;
            result.previous[i] = -1;
            result.valid[i] = 0;
        }
        return result;
    }
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < MAX_NODES; i++) {
        result.distances[i] = INT_MAX;
        result.previous[i] = -1;
        result.valid[i] = 0;
    }
    for (int i = 0; i < g->node_count; i++) {
        result.valid[g->nodes[i].id] = 1;
    }
    result.distances[start] = 0;
    for (int count = 0; count < g->node_count; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        for (int i = 0; i < MAX_NODES; i++) {
            if (result.valid[i] && !visited[i] && result.distances[i] < min_dist) {
                min_dist = result.distances[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < g->edge_count; i++) {