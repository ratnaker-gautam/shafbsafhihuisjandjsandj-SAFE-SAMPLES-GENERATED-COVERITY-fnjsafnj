//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Node {
    int id;
    int visited;
    int distance;
};

struct Graph {
    struct Node *nodes[MAX_NODES];
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
};

void initialize_graph(struct Graph *graph) {
    if (graph == NULL) return;
    
    graph->node_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        *(graph->nodes + i) = NULL;
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(graph->adjacency + i) + j) = 0;
        }
    }
}

int add_node(struct Graph *graph, int id) {
    if (graph == NULL || graph->node_count >= MAX_NODES) return -1;
    if (id < 0 || id >= MAX_NODES) return -1;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (*(graph->nodes + i) != NULL && (*(graph->nodes + i))->id == id) {
            return -1;
        }
    }
    
    struct Node *new_node = malloc(sizeof(struct Node));
    if (new_node == NULL) return -1;
    
    new_node->id = id;
    new_node->visited = 0;
    new_node->distance = INT_MAX;
    
    *(graph->nodes + graph->node_count) = new_node;
    graph->node_count++;
    return 0;
}

int add_edge(struct Graph *graph, int from, int to, int weight) {
    if (graph == NULL) return -1;
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return -1;
    if (weight < 0) return -1;
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (*(graph->nodes + i) != NULL && (*(graph->nodes + i))->id == from) from_exists = 1;
        if (*(graph->nodes + i) != NULL && (*(graph->nodes + i))->id == to) to_exists = 1;
    }
    
    if (!from_exists || !to_exists) return -1;
    
    *(*(graph->adjacency + from) + to) = weight;
    return 0;
}

struct Node* find_min_distance_node(struct Graph *graph) {
    if (graph == NULL) return NULL;
    
    struct Node *min_node = NULL;
    int min_distance = INT_MAX;
    
    for (int i = 0; i < graph->node_count; i++) {
        struct Node *current = *(graph->nodes + i);
        if (current != NULL && !current->visited && current->distance < min_distance) {
            min_distance = current->distance;
            min_node = current;
        }
    }
    
    return min_node;
}

void dijkstra(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        struct Node *current = *(graph->nodes + i);
        if (current != NULL) {
            current->distance = INT_MAX;
            current->visited = 0;
        }
    }
    
    for (int i = 0; i < graph->node_count; i++) {
        struct Node *current = *(graph->nodes + i);
        if (current != NULL && current->id == start_id) {
            current->distance = 0;
            break;
        }
    }
    
    for (int count = 0; count < graph->node_count; count++) {
        struct Node *current = find_min_distance_node(graph);
        if (current == NULL) break;
        
        current->visited = 1;
        
        for (int i = 0; i < graph->node_count; i++) {
            struct Node *neighbor = *(graph->nodes + i);
            if (neighbor != NULL && !neighbor->visited) {
                int edge_weight = *(*(graph->adjacency + current->id) + neighbor->id);
                if (edge_weight > 0) {
                    int new_distance = current->distance + edge_weight;
                    if (new_distance > 0 && new_distance < neighbor->distance) {
                        neighbor->distance = new_distance;
                    }
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *graph, int start_id) {
    if (graph == NULL) return;
    
    printf("Shortest paths from node %d:\n", start_id);
    for (int i = 0; i < graph->node_count; i++) {
        struct Node *current = *(graph->nodes + i);
        if (current != NULL) {
            printf("Node %d: ", current->id);
            if (current->distance == INT_MAX) {
                printf("Unreachable\n");
            } else {
                printf("%d\n", current->distance);
            }
        }
    }
}

void cleanup_graph(struct Graph *graph) {
    if (graph == NULL) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (*(graph->nodes + i