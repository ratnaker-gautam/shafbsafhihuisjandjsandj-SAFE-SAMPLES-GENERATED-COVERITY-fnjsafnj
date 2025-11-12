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

struct QueueNode {
    struct Node* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
    int size;
};

void init_graph(struct Graph* graph) {
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
    graph->node_count++;
    return 1;
}

int add_edge(struct Graph* graph, int from_id, int to_id, int weight) {
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    struct Node* from_node = NULL;
    struct Node* to_node = NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == from_id) from_node = &graph->nodes[i];
        if (graph->nodes[i].id == to_id) to_node = &graph->nodes[i];
    }
    
    if (from_node == NULL || to_node == NULL) return 0;
    
    graph->edges[graph->edge_count].from = from_node;
    graph->edges[graph->edge_count].to = to_node;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void init_queue(struct Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int enqueue(struct Queue* q, struct Node* node) {
    if (q->size >= MAX_NODES) return 0;
    struct QueueNode* new_node = malloc(sizeof(struct QueueNode));
    if (new_node == NULL) return 0;
    new_node->data = node;
    new_node->next = NULL;
    
    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
    return 1;
}

struct Node* dequeue(struct Queue* q) {
    if (q->front == NULL) return NULL;
    struct QueueNode* temp = q->front;
    struct Node* data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    q->size--;
    return data;
}

int is_queue_empty(struct Queue* q) {
    return q->front == NULL;
}

void reset_visited(struct Graph* graph) {
    for (int i = 0; i < graph->node_count; i++) {
        graph->nodes[i].visited = 0;
    }
}

void bfs(struct Graph* graph, int start_id) {
    reset_visited(graph);
    struct Node* start_node = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == start_id) {
            start_node = &graph->nodes[i];
            break;
        }
    }
    if (start_node == NULL) return;
    
    struct Queue q;
    init_queue(&q);
    start_node->visited = 1;
    enqueue(&q, start_node);
    
    printf("BFS traversal: ");
    while (!is_queue_empty(&q)) {
        struct Node* current = dequeue(&q);
        printf("%d ", current->id);
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !graph->edges[i].to->visited) {
                graph->edges[i].to->visited = 1;
                enqueue(&q, graph->edges[i].to);
            }
        }
    }
    printf("\n");
}

void dijkstra(struct Graph* graph, int start_id) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    
    struct Node* start_node = NULL;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->nodes[i].id == start_id) {
            start_node = &graph->nodes[i];
            break;
        }
    }
    if (start_node == NULL) return;
    
    dist[start_node->id] = 0