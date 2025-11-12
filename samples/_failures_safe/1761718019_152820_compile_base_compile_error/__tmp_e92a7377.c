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
    struct Node* source;
    struct Node* target;
    int weight;
};

struct Graph {
    struct Node nodes[MAX_NODES];
    struct Edge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct Path {
    int distance;
    int node_ids[MAX_NODES];
    int path_length;
};

void initialize_graph(struct Graph* graph) {
    if (graph == NULL) return;
    graph->node_count = 0;
    graph->edge_count = 0;
    memset(graph->nodes, 0, sizeof(graph->nodes));
    memset(graph->edges, 0, sizeof(graph->edges));
}

int add_node(struct Graph* graph, int id, const char* label) {
    if (graph == NULL || label == NULL) return 0;
    if (graph->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) return 0;
    }
    
    struct Node* node = &graph->nodes[graph->node_count];
    node->id = id;
    strncpy(node->label, label, sizeof(node->label) - 1);
    node->label[sizeof(node->label) - 1] = '\0';
    graph->node_count++;
    return 1;
}

int add_edge(struct Graph* graph, int source_id, int target_id, int weight) {
    if (graph == NULL) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    struct Node* source = NULL;
    struct Node* target = NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == source_id) source = &graph->nodes[i];
        if (graph->nodes[i].id == target_id) target = &graph->nodes[i];
    }
    
    if (source == NULL || target == NULL) return 0;
    
    struct Edge* edge = &graph->edges[graph->edge_count];
    edge->source = source;
    edge->target = target;
    edge->weight = weight;
    graph->edge_count++;
    return 1;
}

struct Node* find_node_by_id(struct Graph* graph, int id) {
    if (graph == NULL) return NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) {
            return &graph->nodes[i];
        }
    }
    return NULL;
}

int dijkstra_shortest_path(struct Graph* graph, int start_id, int end_id, struct Path* path) {
    if (graph == NULL || path == NULL) return 0;
    if (find_node_by_id(graph, start_id) == NULL) return 0;
    if (find_node_by_id(graph, end_id) == NULL) return 0;
    
    int distances[MAX_NODES];
    int previous[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < MAX_NODES; i++) {
        distances[i] = INT_MAX;
        previous[i] = -1;
        visited[i] = 0;
    }
    
    distances[start_id] = 0;
    
    for (int count = 0; count < graph->node_count; count++) {
        int min_distance = INT_MAX;
        int current_id = -1;
        
        for (int i = 0; i < graph->node_count; i++) {
            int node_id = graph->nodes[i].id;
            if (!visited[node_id] && distances[node_id] < min_distance) {
                min_distance = distances[node_id];
                current_id = node_id;
            }
        }
        
        if (current_id == -1 || current_id == end_id) break;
        visited[current_id] = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source->id == current_id) {
                int neighbor_id = graph->edges[i].target->id;
                int alt_distance;
                if (distances[current_id] == INT_MAX) {
                    alt_distance = INT_MAX;
                } else {
                    alt_distance = distances[current_id] + graph->edges[i].weight;
                    if (alt_distance < distances[current_id]) alt_distance = INT_MAX;
                }
                
                if (alt_distance < distances[neighbor_id]) {
                    distances[neighbor_id] = alt_distance;
                    previous[neighbor_id] = current_id;
                }
            }
        }
    }
    
    if (distances[end_id] == INT_MAX) return 0;
    
    path->distance = distances[end_id];
    path->path_length = 0;
    
    int current = end_id;
    while (current != -1) {
        if (path->path_length >= MAX_NODES) return 0;
        path->node_ids[path->path_length++] = current;
        current = previous[current];