//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 5000

struct Node {
    int id;
    char label[32];
    int visited;
    int distance;
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

struct QueueNode {
    int value;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

void initialize_graph(struct Graph* graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
}

int add_node(struct Graph* graph, const char* label) {
    if (graph->node_count >= MAX_NODES) return -1;
    if (strlen(label) >= 32) return -1;
    
    struct Node* node = &graph->nodes[graph->node_count];
    node->id = graph->node_count;
    strncpy(node->label, label, 31);
    node->label[31] = '\0';
    node->visited = 0;
    node->distance = INT_MAX;
    
    return graph->node_count++;
}

int add_edge(struct Graph* graph, int source, int dest, int weight) {
    if (source < 0 || source >= graph->node_count) return -1;
    if (dest < 0 || dest >= graph->node_count) return -1;
    if (graph->edge_count >= MAX_EDGES) return -1;
    if (weight < 0) return -1;
    
    struct Edge* edge = &graph->edges[graph->edge_count];
    edge->source = source;
    edge->destination = dest;
    edge->weight = weight;
    
    return graph->edge_count++;
}

void initialize_queue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue* queue, int value) {
    if (queue->size >= MAX_NODES) return -1;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (!new_node) return -1;
    
    new_node->value = value;
    new_node->next = NULL;
    
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }
    queue->rear = new_node;
    queue->size++;
    
    return 0;
}

int dequeue(struct Queue* queue) {
    if (!queue->front) return -1;
    
    struct QueueNode* temp = queue->front;
    int value = temp->value;
    queue->front = queue->front->next;
    
    if (!queue->front) {
        queue->rear = NULL;
    }
    
    free(temp);
    queue->size--;
    return value;
}

int is_queue_empty(struct Queue* queue) {
    return queue->front == NULL;
}

void bfs(struct Graph* graph, int start_node) {
    if (start_node < 0 || start_node >= graph->node_count) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
    }
    
    struct Queue queue;
    initialize_queue(&queue);
    
    graph->nodes[start_node].visited = 1;
    graph->nodes[start_node].distance = 0;
    enqueue(&queue, start_node);
    
    while (!is_queue_empty(&queue)) {
        int current = dequeue(&queue);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source == current) {
                int neighbor = graph->edges[i].destination;
                if (!graph->nodes[neighbor].visited) {
                    graph->nodes[neighbor].visited = 1;
                    graph->nodes[neighbor].distance = graph->nodes[current].distance + 1;
                    enqueue(&queue, neighbor);
                }
            }
        }
    }
    
    while (!is_queue_empty(&queue)) {
        dequeue(&queue);
    }
}

void print_bfs_results(struct Graph* graph, int start_node) {
    printf("BFS starting from node %d (%s):\n", start_node, graph->nodes[start_node].label);
    printf("Node ID | Label       | Distance | Reachable\n");
    printf("--------|-------------|----------|----------\n");
    
    for (int i = 0; i < graph->node_count; i++) {
        struct Node* node = &graph->nodes[i];
        char* reachable = (node->distance != INT_MAX) ? "Yes" : "No";
        int distance = (node->distance != INT_MAX) ? node->distance : -1;
        printf("%7d | %-11s | %8d | %-9s\n", 
               node->id, node->label, distance, reachable);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    add_node(&graph, "A");
    add_node(&graph, "B");
    add_node(&graph, "C");
    add_node(&graph, "D");
    add_node