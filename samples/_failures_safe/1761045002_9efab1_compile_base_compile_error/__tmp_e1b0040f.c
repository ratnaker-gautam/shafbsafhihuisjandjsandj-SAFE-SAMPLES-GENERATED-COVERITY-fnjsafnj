//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
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
    int visited;
    int distance;
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

void initialize_graph(struct Graph* graph) {
    if (graph == NULL) return;
    graph->node_count = 0;
    graph->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = -1;
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
        memset(graph->nodes[i].label, 0, sizeof(graph->nodes[i].label));
    }
    for (int i = 0; i < MAX_EDGES; i++) {
        graph->edges[i].source = NULL;
        graph->edges[i].target = NULL;
        graph->edges[i].weight = 0;
    }
}

struct Node* add_node(struct Graph* graph, int id, const char* label) {
    if (graph == NULL || id < 0 || label == NULL) return NULL;
    if (graph->node_count >= MAX_NODES) return NULL;
    if (strlen(label) >= 32) return NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) return NULL;
    }
    
    struct Node* node = &graph->nodes[graph->node_count];
    node->id = id;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    node->visited = 0;
    node->distance = INT_MAX;
    graph->node_count++;
    return node;
}

int add_edge(struct Graph* graph, int source_id, int target_id, int weight) {
    if (graph == NULL || weight < 0) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    
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
        if (graph->nodes[i].id == id) return &graph->nodes[i];
    }
    return NULL;
}

void dijkstra(struct Graph* graph, int start_id) {
    if (graph == NULL) return;
    
    struct Node* start = find_node_by_id(graph, start_id);
    if (start == NULL) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].distance = INT_MAX;
        graph->nodes[i].visited = 0;
    }
    
    start->distance = 0;
    
    for (int count = 0; count < graph->node_count - 1; count++) {
        struct Node* current = NULL;
        int min_distance = INT_MAX;
        
        for (int i = 0; i < graph->node_count; i++) {
            if (!graph->nodes[i].visited && graph->nodes[i].distance < min_distance) {
                min_distance = graph->nodes[i].distance;
                current = &graph->nodes[i];
            }
        }
        
        if (current == NULL) break;
        current->visited = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source == current) {
                struct Node* neighbor = graph->edges[i].target;
                int new_distance;
                if (current->distance == INT_MAX) {
                    new_distance = INT_MAX;
                } else {
                    new_distance = current->distance + graph->edges[i].weight;
                    if (new_distance < 0) new_distance = INT_MAX;
                }
                if (new_distance < neighbor->distance) {
                    neighbor->distance = new_distance;
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph* graph, int start_id) {
    if (graph == NULL) return;
    
    struct Node* start = find_node_by_id(graph, start_id);
    if (start == NULL) {
        printf("Start node %d not found\n", start_id);
        return;
    }
    
    printf("Shortest paths from node %d (%s):\n", start_id, start->label);
    for (int i = 0; i < graph->node_count;