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

struct PathResult {
    int distance;
    int path[MAX_NODES];
    int path_length;
};

void graph_init(struct Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
}

int graph_add_node(struct Graph *g, const char *label) {
    if (g->node_count >= MAX_NODES) return -1;
    if (label == NULL) return -1;
    
    struct Node *node = &g->nodes[g->node_count];
    node->id = g->node_count;
    strncpy(node->label, label, sizeof(node->label) - 1);
    node->label[sizeof(node->label) - 1] = '\0';
    
    return g->node_count++;
}

int graph_add_edge(struct Graph *g, int from, int to, int weight) {
    if (g->edge_count >= MAX_EDGES) return -1;
    if (from < 0 || from >= g->node_count) return -1;
    if (to < 0 || to >= g->node_count) return -1;
    if (from == to) return -1;
    
    struct Edge *edge = &g->edges[g->edge_count];
    edge->from = from;
    edge->to = to;
    edge->weight = weight;
    
    g->edge_count++;
    return 0;
}

int graph_find_shortest_path(struct Graph *g, int start, int end, struct PathResult *result) {
    if (g == NULL || result == NULL) return -1;
    if (start < 0 || start >= g->node_count) return -1;
    if (end < 0 || end >= g->node_count) return -1;
    
    int distances[MAX_NODES];
    int previous[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
        previous[i] = -1;
        visited[i] = 0;
    }
    
    distances[start] = 0;
    
    for (int count = 0; count < g->node_count - 1; count++) {
        int min_distance = INT_MAX;
        int current = -1;
        
        for (int i = 0; i < g->node_count; i++) {
            if (!visited[i] && distances[i] < min_distance) {
                min_distance = distances[i];
                current = i;
            }
        }
        
        if (current == -1) break;
        if (current == end) break;
        
        visited[current] = 1;
        
        for (int i = 0; i < g->edge_count; i++) {
            if (g->edges[i].from == current) {
                int neighbor = g->edges[i].to;
                int new_distance = distances[current] + g->edges[i].weight;
                
                if (new_distance > 0 && new_distance < distances[neighbor]) {
                    distances[neighbor] = new_distance;
                    previous[neighbor] = current;
                }
            }
        }
    }
    
    if (distances[end] == INT_MAX) {
        return -1;
    }
    
    result->distance = distances[end];
    result->path_length = 0;
    
    int current = end;
    while (current != -1) {
        if (result->path_length >= MAX_NODES) return -1;
        result->path[result->path_length++] = current;
        current = previous[current];
    }
    
    for (int i = 0; i < result->path_length / 2; i++) {
        int temp = result->path[i];
        result->path[i] = result->path[result->path_length - 1 - i];
        result->path[result->path_length - 1 - i] = temp;
    }
    
    return 0;
}

void print_graph_info(struct Graph *g) {
    printf("Graph has %d nodes and %d edges\n", g->node_count, g->edge_count);
    printf("Nodes:\n");
    for (int i = 0; i < g->node_count; i++) {
        printf("  %d: %s\n", g->nodes[i].id, g->nodes[i].label);
    }
    printf("Edges:\n");
    for (int i = 0; i < g->edge_count; i++) {
        printf("  %s -> %s (weight: %d)\n", 
               g->nodes[g->edges[i].from].label,
               g->nodes[g->edges[i].to].label,
               g->edges[i].weight);
    }
}

int main(void) {
    struct Graph g;
    graph_init(&g);
    
    graph_add_node(&g, "A");
    graph_add_node(&g, "B");
    graph_add_node(&g, "C");
    graph_add_node(&g, "D");
    graph_add_node(&g,