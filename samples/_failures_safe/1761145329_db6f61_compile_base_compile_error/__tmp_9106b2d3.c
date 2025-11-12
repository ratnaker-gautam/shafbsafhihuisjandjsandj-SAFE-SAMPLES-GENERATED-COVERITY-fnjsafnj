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
        printf("  %s -> %s (%d)\n", 
               g->nodes[g->edges[i].from].label,
               g->nodes[g->edges[i].to].label,
               g->edges[i].weight);
    }
}

int graph_dijkstra(struct Graph *g, int start, int end, int *path, int *path_len) {
    if (g == NULL || path == NULL || path_len == NULL) return -1;
    if (start < 0 || start >= g->node_count) return -1;
    if (end < 0 || end >= g->node_count) return -1;
    
    int dist[MAX_NODES];
    int prev[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count; count++) {
        int min_dist = INT_MAX;
        int u = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || u == end) break;
        visited[u] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == u) {
                int v = g->edges[i].to;
                int alt;
                if (dist[u] > INT_MAX - g->edges[i].weight) continue;
                alt = dist[u] + g->edges[i].weight;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }
    
    if (dist[end] == INT_MAX) return -1;
    
    int current = end;
    int temp_path[MAX_NODES];
    int temp_len = 0;
    
    while (current != -1) {
        if (temp_len >= MAX_NODES) return -1;
        temp_path[temp_len++] = current;
        current = prev[current];
    }
    
    if (temp_len > *path_len) return -1;
    
    for (int i = 0; i < temp_len; i++) {
        path[i] = temp_path[temp_len - 1 - i];
    }
    *path_len = temp_len;
    
    return dist[end];
}

int main(void) {
    struct Graph g;
    graph_init(&g);
    
    int a = graph_add_node(&g, "A");
    int b = graph_add_node(&g, "B");
    int c = graph_add_node(&g, "C");
    int d = graph_add_node(&g, "D");
    int e = graph_add_node(&g, "E");
    
    if (a < 0 || b