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

void graph_init(struct Graph* graph) {
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

int graph_add_node(struct Graph* graph, const char* label) {
    if (graph == NULL || label == NULL) return -1;
    if (graph->node_count >= MAX_NODES) return -1;
    if (strlen(label) >= sizeof(graph->nodes[0].label)) return -1;
    
    int id = graph->node_count;
    graph->nodes[id].id = id;
    graph->nodes[id].visited = 0;
    graph->nodes[id].distance = INT_MAX;
    strncpy(graph->nodes[id].label, label, sizeof(graph->nodes[id].label) - 1);
    graph->nodes[id].label[sizeof(graph->nodes[id].label) - 1] = '\0';
    graph->node_count++;
    return id;
}

int graph_add_edge(struct Graph* graph, int source_id, int target_id, int weight) {
    if (graph == NULL) return -1;
    if (source_id < 0 || source_id >= graph->node_count) return -1;
    if (target_id < 0 || target_id >= graph->node_count) return -1;
    if (graph->edge_count >= MAX_EDGES) return -1;
    if (weight < 0) return -1;
    
    struct Edge* edge = &graph->edges[graph->edge_count];
    edge->source = &graph->nodes[source_id];
    edge->target = &graph->nodes[target_id];
    edge->weight = weight;
    graph->edge_count++;
    return graph->edge_count - 1;
}

void graph_reset_visited(struct Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
    }
}

void graph_dijkstra(struct Graph* graph, int start_id) {
    if (graph == NULL) return;
    if (start_id < 0 || start_id >= graph->node_count) return;
    
    graph_reset_visited(graph);
    graph->nodes[start_id].distance = 0;
    
    for (int i = 0; i < graph->node_count; i++) {
        int min_distance = INT_MAX;
        int current_id = -1;
        
        for (int j = 0; j < graph->node_count; j++) {
            if (!graph->nodes[j].visited && graph->nodes[j].distance < min_distance) {
                min_distance = graph->nodes[j].distance;
                current_id = j;
            }
        }
        
        if (current_id == -1) break;
        graph->nodes[current_id].visited = 1;
        
        for (int j = 0; j < graph->edge_count; j++) {
            if (graph->edges[j].source->id == current_id) {
                int target_id = graph->edges[j].target->id;
                int new_distance;
                if (graph->nodes[current_id].distance == INT_MAX) {
                    new_distance = INT_MAX;
                } else {
                    new_distance = graph->nodes[current_id].distance + graph->edges[j].weight;
                    if (new_distance < graph->nodes[current_id].distance) new_distance = INT_MAX;
                }
                
                if (new_distance < graph->nodes[target_id].distance) {
                    graph->nodes[target_id].distance = new_distance;
                }
            }
        }
    }
}

void graph_print_distances(struct Graph* graph) {
    if (graph == NULL) return;
    
    printf("Node distances:\n");
    for (int i = 0; i < graph->node_count; i++) {
        printf("  %s (id %d): ", graph->nodes[i].label, graph->nodes[i].id);
        if (graph->nodes[i].distance == INT_MAX) {
            printf("unreachable\n");
        } else {
            printf("%d\n", graph->nodes[i].distance);
        }
    }
}

int main() {
    struct Graph graph;
    graph_init(&graph);
    
    int a = graph_add_node(&graph, "A");
    int b = graph_add_node(&graph, "B");
    int c =