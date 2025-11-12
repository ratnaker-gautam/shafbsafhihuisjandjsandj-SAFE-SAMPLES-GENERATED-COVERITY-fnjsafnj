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
    if (g == NULL) return 0;
    if (from < 0 || from >= g->node_count) return 0;
    if (to < 0 || to >= g->node_count) return 0;
    if (g->edge_count >= MAX_EDGES) return 0;
    
    struct Edge *e = &g->edges[g->edge_count];
    e->from = from;
    e->to = to;
    e->weight = weight;
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
        printf("  %s -> %s (weight: %d)\n", 
               g->nodes[g->edges[i].from].label,
               g->nodes[g->edges[i].to].label,
               g->edges[i].weight);
    }
}

int graph_find_node_by_label(struct Graph *g, const char *label) {
    if (g == NULL || label == NULL) return -1;
    
    for (int i = 0; i < g->node_count; i++) {
        if (strcmp(g->nodes[i].label, label) == 0) {
            return i;
        }
    }
    return -1;
}

void graph_dijkstra(struct Graph *g, int start, int dist[], int prev[]) {
    if (g == NULL || dist == NULL || prev == NULL) return;
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int u = -1;
        
        for (int v = 0; v < g->node_count; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        visited[u] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == u) {
                int v = g->edges[i].to;
                int alt;
                if (dist[u] < INT_MAX - g->edges[i].weight) {
                    alt = dist[u] + g->edges[i].weight;
                } else {
                    alt = INT_MAX;
                }
                if (!visited[v] && alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }
}

void print_shortest_path(struct Graph *g, int start, int target, int prev[]) {
    if (g == NULL || prev == NULL) return;
    if (start < 0 || start >= g->node_count) return;
    if (target < 0 || target >= g->node_count) return;
    
    if (target == start) {
        printf("%s", g->nodes[start].label);
        return;
    }
    
    if (prev[target] == -1) {
        printf("No path");
        return;
    }
    
    print_shortest_path(g, start, prev[target], prev);
    printf(" -> %s", g->nodes[target].label);
}

int main() {
    struct Graph g;
    graph_init(&g);
    
    graph_add_node(&g, "A");
    graph_add_node(&g, "B