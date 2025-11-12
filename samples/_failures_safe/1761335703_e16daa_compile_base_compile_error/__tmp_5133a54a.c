//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Node {
    int id;
    int visited;
};

struct Edge {
    struct Node* from;
    struct Node* to;
    int weight;
};

struct Graph {
    struct Node nodes[MAX_NODES];
    struct Edge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

void init_graph(struct Graph* graph) {
    if (graph == NULL) return;
    graph->node_count = 0;
    graph->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = -1;
        graph->nodes[i].visited = 0;
    }
}

int add_node(struct Graph* graph, int id) {
    if (graph == NULL || id < 0 || id >= MAX_NODES) return -1;
    if (graph->node_count >= MAX_NODES) return -1;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) return -1;
    }
    graph->nodes[graph->node_count].id = id;
    graph->nodes[graph->node_count].visited = 0;
    graph->node_count++;
    return 0;
}

int add_edge(struct Graph* graph, int from_id, int to_id, int weight) {
    if (graph == NULL || weight < 0) return -1;
    if (graph->edge_count >= MAX_EDGES) return -1;
    
    struct Node* from_node = NULL;
    struct Node* to_node = NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == from_id) from_node = &graph->nodes[i];
        if (graph->nodes[i].id == to_id) to_node = &graph->nodes[i];
    }
    
    if (from_node == NULL || to_node == NULL) return -1;
    
    graph->edges[graph->edge_count].from = from_node;
    graph->edges[graph->edge_count].to = to_node;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 0;
}

void reset_visited(struct Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
    }
}

int find_node_index(struct Graph* graph, int id) {
    if (graph == NULL) return -1;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) return i;
    }
    return -1;
}

void dijkstra(struct Graph* graph, int start_id) {
    if (graph == NULL) return;
    int start_idx = find_node_index(graph, start_id);
    if (start_idx == -1) return;
    
    int dist[MAX_NODES];
    int prev[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        graph->nodes[i].visited = 0;
    }
    
    dist[start_idx] = 0;
    
    for (int count = 0; count < graph->node_count; count++) {
        int min_dist = INT_MAX;
        int min_index = -1;
        
        for (int i = 0; i < graph->node_count; i++) {
            if (!graph->nodes[i].visited && dist[i] < min_dist) {
                min_dist = dist[i];
                min_index = i;
            }
        }
        
        if (min_index == -1) break;
        graph->nodes[min_index].visited = 1;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from->id == graph->nodes[min_index].id) {
                int to_idx = find_node_index(graph, graph->edges[i].to->id);
                if (to_idx == -1) continue;
                
                if (dist[min_index] != INT_MAX) {
                    long new_dist = (long)dist[min_index] + graph->edges[i].weight;
                    if (new_dist > INT_MAX) continue;
                    if (new_dist < dist[to_idx]) {
                        dist[to_idx] = new_dist;
                        prev[to_idx] = min_index;
                    }
                }
            }
        }
    }
    
    printf("Shortest paths from node %d:\n", start_id);
    for (int i = 0; i < graph->node_count; i++) {
        printf("Node %d: ", graph->nodes[i].id);
        if (dist[i] == INT_MAX) {
            printf("Unreachable\n");
        } else {
            printf("%d\n", dist[i]);
        }
    }
}

int main() {
    struct Graph graph;
    init_graph(&graph);
    
    printf("Adding nodes 0 through 5...\n");
    for (int i = 0; i < 6; i++) {
        if (add_node(&graph, i) != 0) {
            printf("Failed to add node %d\n", i);
            return