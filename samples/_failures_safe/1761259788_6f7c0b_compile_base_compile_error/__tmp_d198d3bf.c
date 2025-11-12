//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Node {
    int id;
    char label[32];
};

struct Edge {
    int source;
    int destination;
    int weight;
};

struct Graph {
    struct Node nodes[MAX_NODES];
    struct Edge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct Graph* create_graph() {
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->node_count = 0;
    graph->edge_count = 0;
    return graph;
}

int add_node(struct Graph* graph, int id, const char* label) {
    if (graph == NULL || graph->node_count >= MAX_NODES) {
        return 0;
    }
    if (id < 0 || id >= MAX_NODES) {
        return 0;
    }
    if (label == NULL || strlen(label) >= 32) {
        return 0;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) {
            return 0;
        }
    }
    graph->nodes[graph->node_count].id = id;
    strncpy(graph->nodes[graph->node_count].label, label, 31);
    graph->nodes[graph->node_count].label[31] = '\0';
    graph->node_count++;
    return 1;
}

int add_edge(struct Graph* graph, int source, int dest, int weight) {
    if (graph == NULL || graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (source < 0 || source >= MAX_NODES || dest < 0 || dest >= MAX_NODES) {
        return 0;
    }
    int source_exists = 0;
    int dest_exists = 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == source) source_exists = 1;
        if (graph->nodes[i].id == dest) dest_exists = 1;
    }
    if (!source_exists || !dest_exists) {
        return 0;
    }
    graph->edges[graph->edge_count].source = source;
    graph->edges[graph->edge_count].destination = dest;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(struct Graph* graph, int start) {
    if (graph == NULL || graph->node_count == 0) {
        return;
    }
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;
    for (int count = 0; count < graph->node_count; count++) {
        int min_dist = INT_MAX;
        int min_node = -1;
        for (int i = 0; i < graph->node_count; i++) {
            int node_id = graph->nodes[i].id;
            if (!visited[node_id] && dist[node_id] <= min_dist) {
                min_dist = dist[node_id];
                min_node = node_id;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source == min_node) {
                int dest = graph->edges[i].destination;
                int weight = graph->edges[i].weight;
                if (weight < 0) continue;
                if (dist[min_node] != INT_MAX && dist[min_node] + weight < dist[dest]) {
                    dist[dest] = dist[min_node] + weight;
                }
            }
        }
    }
    printf("Shortest paths from node %d:\n", start);
    for (int i = 0; i < graph->node_count; i++) {
        int node_id = graph->nodes[i].id;
        if (dist[node_id] == INT_MAX) {
            printf("Node %d: unreachable\n", node_id);
        } else {
            printf("Node %d: distance %d\n", node_id, dist[node_id]);
        }
    }
}

void free_graph(struct Graph* graph) {
    if (graph != NULL) {
        free(graph);
    }
}

int main() {
    struct Graph* graph = create_graph();
    if (graph == NULL) {
        printf("Failed to create graph\n");
        return 1;
    }
    add_node(graph, 0, "A");
    add_node(graph, 1, "B");
    add_node(graph, 2, "C");
    add_node(graph, 3, "D");
    add_node(graph, 4, "E");
    add_edge(graph, 0, 1, 4);
    add_edge(graph, 0, 2, 2);
    add_edge(graph, 1, 2, 1);
    add_edge(graph, 1, 3, 5);
    add_edge(graph, 2, 3, 8);
    add_edge(graph, 2, 4, 10);
    di