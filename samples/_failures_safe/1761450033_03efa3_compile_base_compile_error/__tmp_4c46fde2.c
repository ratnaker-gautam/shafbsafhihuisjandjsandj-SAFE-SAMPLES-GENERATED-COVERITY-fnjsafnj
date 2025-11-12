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

void dijkstra(struct Graph *g, int start) {
    if (g == NULL) return;
    if (start < 0 || start >= g->node_count) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && dist[i] <= min_dist) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        
        if (min_node == -1) break;
        visited[min_node] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == min_node) {
                int to = g->edges[i].to;
                int weight = g->edges[i].weight;
                if (weight < 0) continue;
                if (!visited[to] && dist[min_node] != INT_MAX) {
                    int new_dist = dist[min_node] + weight;
                    if (new_dist < dist[to]) {
                        dist[to] = new_dist;
                    }
                }
            }
        }
    }
    
    printf("Shortest paths from %s:\n", g->nodes[start].label);
    for (int i = 0; i < g->node_count; i++) {
        if (dist[i] == INT_MAX) {
            printf("  to %s: unreachable\n", g->nodes[i].label);
        } else {
            printf("  to %s: %d\n", g->nodes[i].label, dist[i]);
        }
    }
}

int main() {
    struct Graph g;
    graph_init(&g);
    
    graph_add_node(&g, "A");
    graph_add_node(&g, "B");
    graph_add_node(&g, "C");
    graph_add_node(&g, "D");
    graph_add_node(&g, "E");
    
    graph_add_edge(&g, 0, 1, 4);
    graph_add_edge(&g, 0, 2, 2);
    graph_add_edge(&g, 1, 2, 1);
    graph_add_edge(&g, 1, 3, 5);
    graph_add_edge(&g, 2, 3, 8);
    graph_add_edge(&g, 2, 4, 10);
    graph_add_edge(&g, 3, 4, 2);
    
    graph