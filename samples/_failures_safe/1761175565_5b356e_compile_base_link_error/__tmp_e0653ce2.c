//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct GraphNode {
    int id;
    int visited;
    struct GraphEdge *edges;
    int edge_count;
};

struct GraphEdge {
    struct GraphNode *target;
    int weight;
};

struct Graph {
    struct GraphNode *nodes;
    int node_count;
    int capacity;
};

struct QueueNode {
    struct GraphNode *data;
    struct QueueNode *next;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
    int size;
};

struct StackNode {
    struct GraphNode *data;
    struct StackNode *next;
};

struct Stack {
    struct StackNode *top;
    int size;
};

struct Graph graph_create(int initial_capacity) {
    struct Graph g;
    g.nodes = NULL;
    g.node_count = 0;
    g.capacity = 0;
    
    if (initial_capacity > 0 && initial_capacity <= 1000) {
        g.nodes = malloc(sizeof(struct GraphNode) * initial_capacity);
        if (g.nodes) {
            g.capacity = initial_capacity;
            for (int i = 0; i < initial_capacity; i++) {
                g.nodes[i].id = -1;
                g.nodes[i].visited = 0;
                g.nodes[i].edges = NULL;
                g.nodes[i].edge_count = 0;
            }
        }
    }
    return g;
}

void graph_destroy(struct Graph *g) {
    if (g) {
        for (int i = 0; i < g->node_count; i++) {
            if (g->nodes[i].edges) {
                free(g->nodes[i].edges);
            }
        }
        if (g->nodes) {
            free(g->nodes);
        }
        g->nodes = NULL;
        g->node_count = 0;
        g->capacity = 0;
    }
}

int graph_add_node(struct Graph *g, int id) {
    if (!g || id < 0 || id >= 1000) return 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == id) return 0;
    }
    
    if (g->node_count >= g->capacity) {
        int new_capacity = g->capacity == 0 ? 4 : g->capacity * 2;
        if (new_capacity > 1000) new_capacity = 1000;
        struct GraphNode *new_nodes = realloc(g->nodes, sizeof(struct GraphNode) * new_capacity);
        if (!new_nodes) return 0;
        g->nodes = new_nodes;
        g->capacity = new_capacity;
        for (int i = g->node_count; i < new_capacity; i++) {
            g->nodes[i].id = -1;
            g->nodes[i].visited = 0;
            g->nodes[i].edges = NULL;
            g->nodes[i].edge_count = 0;
        }
    }
    
    g->nodes[g->node_count].id = id;
    g->nodes[g->node_count].visited = 0;
    g->nodes[g->node_count].edges = NULL;
    g->nodes[g->node_count].edge_count = 0;
    g->node_count++;
    return 1;
}

int graph_add_edge(struct Graph *g, int from_id, int to_id, int weight) {
    if (!g || weight < 0 || weight > 100) return 0;
    
    struct GraphNode *from_node = NULL;
    struct GraphNode *to_node = NULL;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].id == from_id) from_node = &g->nodes[i];
        if (g->nodes[i].id == to_id) to_node = &g->nodes[i];
    }
    
    if (!from_node || !to_node) return 0;
    
    for (int i = 0; i < from_node->edge_count; i++) {
        if (from_node->edges[i].target->id == to_id) return 0;
    }
    
    int new_count = from_node->edge_count + 1;
    struct GraphEdge *new_edges = realloc(from_node->edges, sizeof(struct GraphEdge) * new_count);
    if (!new_edges) return 0;
    
    from_node->edges = new_edges;
    from_node->edges[from_node->edge_count].target = to_node;
    from_node->edges[from_node->edge_count].weight = weight;
    from_node->edge_count++;
    return 1;
}

void graph_reset_visited(struct Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i].visited = 0;
    }
}

struct Queue queue_create(void) {
    struct Queue q;
    q.front = NULL;
    q.rear = NULL;
    q.size = 0;
    return q;
}

void queue_destroy(struct Queue *q) {
    if (!q) return;
    struct QueueNode *current = q->front;
    while (current) {
        struct QueueNode *next = current->next;
        free(current);
        current = next;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}