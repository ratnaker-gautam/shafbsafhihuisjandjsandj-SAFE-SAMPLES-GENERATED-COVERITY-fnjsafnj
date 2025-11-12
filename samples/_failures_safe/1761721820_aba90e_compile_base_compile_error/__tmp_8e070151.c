//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

struct GraphNode {
    int id;
    int visited;
    int distance;
    struct GraphNode* parent;
};

struct GraphEdge {
    struct GraphNode* source;
    struct GraphNode* destination;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    struct GraphEdge edges[MAX_NODES * MAX_NODES];
    int node_count;
    int edge_count;
};

void initialize_graph(struct Graph* graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = i;
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INF;
        graph->nodes[i].parent = NULL;
    }
}

int add_node(struct Graph* graph, int node_id) {
    if (graph->node_count >= MAX_NODES || node_id < 0 || node_id >= MAX_NODES) {
        return 0;
    }
    if (graph->nodes[node_id].distance != INF) {
        return 0;
    }
    graph->nodes[node_id].distance = INF;
    graph->nodes[node_id].visited = 0;
    graph->nodes[node_id].parent = NULL;
    graph->node_count++;
    return 1;
}

int add_edge(struct Graph* graph, int source_id, int dest_id, int weight) {
    if (source_id < 0 || source_id >= MAX_NODES || dest_id < 0 || dest_id >= MAX_NODES) {
        return 0;
    }
    if (graph->nodes[source_id].distance == INF || graph->nodes[dest_id].distance == INF) {
        return 0;
    }
    if (weight < 0) {
        return 0;
    }
    if (graph->edge_count >= MAX_NODES * MAX_NODES) {
        return 0;
    }
    graph->edges[graph->edge_count].source = &graph->nodes[source_id];
    graph->edges[graph->edge_count].destination = &graph->nodes[dest_id];
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(struct Graph* graph, int start_id) {
    if (start_id < 0 || start_id >= MAX_NODES || graph->nodes[start_id].distance == INF) {
        return;
    }
    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].distance = INF;
        graph->nodes[i].visited = 0;
        graph->nodes[i].parent = NULL;
    }
    graph->nodes[start_id].distance = 0;
    for (int count = 0; count < graph->node_count; count++) {
        int min_distance = INF;
        int min_index = -1;
        for (int i = 0; i < MAX_NODES; i++) {
            if (!graph->nodes[i].visited && graph->nodes[i].distance != INF && graph->nodes[i].distance < min_distance) {
                min_distance = graph->nodes[i].distance;
                min_index = i;
            }
        }
        if (min_index == -1) {
            break;
        }
        graph->nodes[min_index].visited = 1;
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source->id == min_index) {
                int dest_id = graph->edges[i].destination->id;
                int new_distance = graph->nodes[min_index].distance + graph->edges[i].weight;
                if (new_distance >= 0 && new_distance < graph->nodes[dest_id].distance) {
                    graph->nodes[dest_id].distance = new_distance;
                    graph->nodes[dest_id].parent = &graph->nodes[min_index];
                }
            }
        }
    }
}

void print_path(struct Graph* graph, int node_id) {
    if (node_id < 0 || node_id >= MAX_NODES || graph->nodes[node_id].distance == INF) {
        printf("No path\n");
        return;
    }
    if (graph->nodes[node_id].parent == NULL) {
        printf("%d", node_id);
        return;
    }
    print_path(graph, graph->nodes[node_id].parent->id);
    printf(" -> %d", node_id);
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    int num_nodes, num_edges;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &num_nodes) != 1 || num_nodes <= 0 || num_nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    for (int i = 0; i < num_nodes; i++) {
        if (!add_node(&graph, i)) {
            printf("Failed to add node %d\n", i);
            return 1;
        }
    }
    printf("Enter number of edges: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0 || num_edges > MAX_NODES * MAX_NODES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    for