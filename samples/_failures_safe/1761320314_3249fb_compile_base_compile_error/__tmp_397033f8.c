//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct GraphNode {
    int id;
    int visited;
    int distance;
};

struct GraphEdge {
    struct GraphNode* source;
    struct GraphNode* destination;
    int weight;
};

struct Graph {
    struct GraphNode nodes[MAX_NODES];
    struct GraphEdge edges[MAX_EDGES];
    int node_count;
    int edge_count;
};

struct QueueNode {
    struct GraphNode* data;
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

int add_node(struct Graph* graph, int id) {
    if (graph->node_count >= MAX_NODES) return 0;
    if (id < 0 || id >= MAX_NODES) return 0;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == id) return 0;
    }
    
    graph->nodes[graph->node_count].id = id;
    graph->nodes[graph->node_count].visited = 0;
    graph->nodes[graph->node_count].distance = INT_MAX;
    graph->node_count++;
    return 1;
}

int add_edge(struct Graph* graph, int source_id, int dest_id, int weight) {
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    struct GraphNode* source = NULL;
    struct GraphNode* dest = NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == source_id) source = &graph->nodes[i];
        if (graph->nodes[i].id == dest_id) dest = &graph->nodes[i];
    }
    
    if (source == NULL || dest == NULL) return 0;
    
    graph->edges[graph->edge_count].source = source;
    graph->edges[graph->edge_count].destination = dest;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void initialize_queue(struct Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int enqueue(struct Queue* queue, struct GraphNode* node) {
    if (queue->size >= MAX_NODES) return 0;
    
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return 0;
    
    new_node->data = node;
    new_node->next = NULL;
    
    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    
    queue->size++;
    return 1;
}

struct GraphNode* dequeue(struct Queue* queue) {
    if (queue->front == NULL) return NULL;
    
    struct QueueNode* temp = queue->front;
    struct GraphNode* data = temp->data;
    
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->rear = NULL;
    
    free(temp);
    queue->size--;
    return data;
}

int is_queue_empty(struct Queue* queue) {
    return queue->front == NULL;
}

void reset_graph(struct Graph* graph) {
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
        graph->nodes[i].distance = INT_MAX;
    }
}

void bfs(struct Graph* graph, int start_id) {
    reset_graph(graph);
    
    struct GraphNode* start = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == start_id) {
            start = &graph->nodes[i];
            break;
        }
    }
    
    if (start == NULL) return;
    
    struct Queue queue;
    initialize_queue(&queue);
    
    start->visited = 1;
    start->distance = 0;
    enqueue(&queue, start);
    
    printf("BFS traversal from node %d: ", start_id);
    
    while (!is_queue_empty(&queue)) {
        struct GraphNode* current = dequeue(&queue);
        printf("%d ", current->id);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].source == current) {
                struct GraphNode* neighbor = graph->edges[i].destination;
                if (!neighbor->visited) {
                    neighbor->visited = 1;
                    neighbor->distance = current->distance + 1;
                    enqueue(&queue, neighbor);
                }
            }
        }
    }
    printf("\n");
}

void free_queue(struct Queue* queue) {
    while (!is_queue_empty(queue)) {
        dequeue(queue);
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Algorithm Demo\n");
    printf("Adding nodes 0-5...\n");