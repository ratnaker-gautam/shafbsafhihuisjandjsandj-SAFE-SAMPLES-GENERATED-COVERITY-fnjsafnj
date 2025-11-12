//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
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
    struct Node nodes[MAX_NODES];
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
};

void initialize_graph(struct Graph *graph) {
    if (graph == NULL) return;
    
    graph->node_count = 0;
    
    for (int i = 0; i < MAX_NODES; i++) {
        (*(graph->nodes + i)).id = i;
        (*(graph->nodes + i)).visited = 0;
        (*(graph->nodes + i)).distance = INT_MAX;
        
        for (int j = 0; j < MAX_NODES; j++) {
            *(*(graph->adjacency + i) + j) = 0;
        }
    }
}

int add_node(struct Graph *graph) {
    if (graph == NULL) return -1;
    if (graph->node_count >= MAX_NODES) return -1;
    
    int new_id = graph->node_count;
    (*(graph->nodes + new_id)).id = new_id;
    (*(graph->nodes + new_id)).visited = 0;
    (*(graph->nodes + new_id)).distance = INT_MAX;
    
    graph->node_count++;
    return new_id;
}

int add_edge(struct Graph *graph, int from, int to, int weight) {
    if (graph == NULL) return 0;
    if (from < 0 || from >= graph->node_count) return 0;
    if (to < 0 || to >= graph->node_count) return 0;
    if (weight < 0) return 0;
    
    *(*(graph->adjacency + from) + to) = weight;
    *(*(graph->adjacency + to) + from) = weight;
    
    return 1;
}

int find_min_distance_node(struct Graph *graph) {
    if (graph == NULL) return -1;
    
    int min_distance = INT_MAX;
    int min_node = -1;
    
    for (int i = 0; i < graph->node_count; i++) {
        struct Node *current = graph->nodes + i;
        if (!current->visited && current->distance < min_distance) {
            min_distance = current->distance;
            min_node = i;
        }
    }
    
    return min_node;
}

void dijkstra(struct Graph *graph, int start) {
    if (graph == NULL) return;
    if (start < 0 || start >= graph->node_count) return;
    
    (*(graph->nodes + start)).distance = 0;
    
    for (int count = 0; count < graph->node_count; count++) {
        int current = find_min_distance_node(graph);
        if (current == -1) break;
        
        (*(graph->nodes + current)).visited = 1;
        
        for (int neighbor = 0; neighbor < graph->node_count; neighbor++) {
            int weight = *(*(graph->adjacency + current) + neighbor);
            if (weight > 0) {
                struct Node *neighbor_node = graph->nodes + neighbor;
                int new_distance = (*(graph->nodes + current)).distance + weight;
                
                if (new_distance < neighbor_node->distance) {
                    neighbor_node->distance = new_distance;
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph *graph) {
    if (graph == NULL) return;
    
    printf("Shortest paths from node 0:\n");
    for (int i = 0; i < graph->node_count; i++) {
        struct Node *node = graph->nodes + i;
        if (node->distance == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: distance %d\n", i, node->distance);
        }
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    for (int i = 0; i < 6; i++) {
        add_node(&graph);
    }
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    
    dijkstra(&graph, 0);
    print_shortest_paths(&graph);
    
    return 0;
}