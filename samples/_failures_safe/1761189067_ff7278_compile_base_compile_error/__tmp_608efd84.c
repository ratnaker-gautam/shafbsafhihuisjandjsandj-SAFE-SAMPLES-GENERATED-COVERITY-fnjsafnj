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

struct Graph* create_graph(void) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->node_count = 0;
    graph->edge_count = 0;
    return graph;
}

int add_node(struct Graph* graph, int id, const char* label) {
    if (graph == NULL || label == NULL) {
        return 0;
    }
    if (graph->node_count >= MAX_NODES) {
        return 0;
    }
    if (id < 0 || id >= MAX_NODES) {
        return 0;
    }
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) {
            return 0;
        }
    }
    struct Node* node = &graph->nodes[graph->node_count];
    node->id = id;
    strncpy(node->label, label, sizeof(node->label) - 1);
    node->label[sizeof(node->label) - 1] = '\0';
    graph->node_count++;
    return 1;
}

int add_edge(struct Graph* graph, int from_id, int to_id, int weight) {
    if (graph == NULL) {
        return 0;
    }
    if (graph->edge_count >= MAX_EDGES) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    struct Node* from_node = NULL;
    struct Node* to_node = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == from_id) {
            from_node = &graph->nodes[i];
        }
        if (graph->nodes[i].id == to_id) {
            to_node = &graph->nodes[i];
        }
    }
    if (from_node == NULL || to_node == NULL) {
        return 0;
    }
    struct Edge* edge = &graph->edges[graph->edge_count];
    edge->from = from_node;
    edge->to = to_node;
    edge->weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(struct Graph* graph, int start_id, int distances[]) {
    if (graph == NULL || distances == NULL) {
        return;
    }
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < MAX_NODES; i++) {
        distances[i] = INT_MAX;
    }
    distances[start_id] = 0;
    for (int count = 0; count < graph->node_count - 1; count++) {
        int min_distance = INT_MAX;
        int min_index = -1;
        for (int i = 0; i < graph->node_count; i++) {
            int node_id = graph->nodes[i].id;
            if (!visited[node_id] && distances[node_id] <= min_distance) {
                min_distance = distances[node_id];
                min_index = node_id;
            }
        }
        if (min_index == -1) {
            break;
        }
        visited[min_index] = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from->id == min_index) {
                int to_id = graph->edges[i].to->id;
                int new_distance = distances[min_index] + graph->edges[i].weight;
                if (new_distance > 0 && new_distance < distances[to_id]) {
                    distances[to_id] = new_distance;
                }
            }
        }
    }
}

void print_shortest_paths(struct Graph* graph, int start_id) {
    if (graph == NULL) {
        return;
    }
    int distances[MAX_NODES];
    dijkstra(graph, start_id, distances);
    printf("Shortest paths from node %d:\n", start_id);
    for (int i = 0; i < graph->node_count; i++) {
        int node_id = graph->nodes[i].id;
        if (distances[node_id] == INT_MAX) {
            printf("Node %d (%s): unreachable\n", node_id, graph->nodes[i].label);
        } else {
            printf("Node %d (%s): distance %d\n", node_id, graph->nodes[i].label, distances[node_id]);
        }
    }
}

int main(void) {
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
    add_edge(graph, 0, 2,