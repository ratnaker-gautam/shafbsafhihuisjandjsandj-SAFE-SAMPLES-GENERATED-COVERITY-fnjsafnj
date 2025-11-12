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
    int predecessor;
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

struct Queue {
    int items[MAX_NODES];
    int front;
    int rear;
};

void initialize_graph(struct Graph *graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
}

int add_node(struct Graph *graph, const char *label) {
    if (graph->node_count >= MAX_NODES) return -1;
    if (strlen(label) >= 32) return -1;
    
    struct Node *node = &graph->nodes[graph->node_count];
    node->id = graph->node_count;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    node->visited = 0;
    node->distance = INT_MAX;
    node->predecessor = -1;
    
    return graph->node_count++;
}

int add_edge(struct Graph *graph, int source, int dest, int weight) {
    if (graph->edge_count >= MAX_EDGES) return -1;
    if (source < 0 || source >= graph->node_count) return -1;
    if (dest < 0 || dest >= graph->node_count) return -1;
    if (source == dest) return -1;
    
    struct Edge *edge = &graph->edges[graph->edge_count];
    edge->source = source;
    edge->destination = dest;
    edge->weight = weight;
    
    return graph->edge_count++;
}

void initialize_queue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int is_queue_empty(struct Queue *q) {
    return q->front == -1;
}

void enqueue(struct Queue *q, int value) {
    if (q->rear == MAX_NODES - 1) return;
    if (q->front == -1) q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(struct Queue *q) {
    if (is_queue_empty(q)) return -1;
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return item;
}

void bfs_shortest_path(struct Graph *graph, int start_node) {
    if (start_node < 0 || start_node >= graph->node_count) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
        graph->nodes[i].predecessor = -1;
    }
    
    struct Queue q;
    initialize_queue(&q);
    
    graph->nodes[start_node].visited = 1;
    graph->nodes[start_node].distance = 0;
    enqueue(&q, start_node);
    
    while (!is_queue_empty(&q)) {
        int current = dequeue(&q);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source == current) {
                int neighbor = graph->edges[i].destination;
                if (!graph->nodes[neighbor].visited) {
                    graph->nodes[neighbor].visited = 1;
                    graph->nodes[neighbor].distance = graph->nodes[current].distance + 1;
                    graph->nodes[neighbor].predecessor = current;
                    enqueue(&q, neighbor);
                }
            }
        }
    }
}

void print_shortest_path(struct Graph *graph, int target) {
    if (target < 0 || target >= graph->node_count) {
        printf("Invalid target node\n");
        return;
    }
    
    if (graph->nodes[target].distance == INT_MAX) {
        printf("No path exists to node %s\n", graph->nodes[target].label);
        return;
    }
    
    printf("Shortest path to %s (distance %d): ", graph->nodes[target].label, graph->nodes[target].distance);
    
    int path[MAX_NODES];
    int path_length = 0;
    int current = target;
    
    while (current != -1) {
        path[path_length++] = current;
        current = graph->nodes[current].predecessor;
    }
    
    for (int i = path_length - 1; i >= 0; i--) {
        printf("%s", graph->nodes[path[i]].label);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    int a = add_node(&graph, "A");
    int b = add_node(&graph, "B");
    int c = add_node(&graph, "C");
    int d = add_node(&graph, "D");
    int e = add_node(&graph, "E");
    
    if (a < 0 ||