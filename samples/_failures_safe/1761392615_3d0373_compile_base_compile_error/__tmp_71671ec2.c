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

void dijkstra(struct Graph *g, int start, int distances[]) {
    if (g == NULL || distances == NULL) return;
    if (start < 0 || start >= g->node_count) return;
    
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && distances[i] < min_dist) {
                min_dist = distances[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == min_node) {
                int to = g->edges[i].to;
                int new_dist;
                if (distances[min_node] == INT_MAX) {
                    new_dist = INT_MAX;
                } else {
                    new_dist = distances[min_node] + g->edges[i].weight;
                    if (new_dist < 0) new_dist = INT_MAX;
                }
                
                if (!visited[to] && new_dist < distances[to]) {
                    distances[to] = new_dist;
                }
            }
        }
    }
}

int main(void) {
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
    
    graph_print(&g);
    
    int distances