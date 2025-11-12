//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100

struct Node {
    int id;
    int visited;
    struct Node** neighbors;
    int neighbor_count;
    int capacity;
};

struct Graph {
    struct Node* nodes[MAX_NODES];
    int node_count;
};

void init_graph(struct Graph* g) {
    g->node_count = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->nodes[i] = NULL;
    }
}

struct Node* create_node(int id) {
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) return NULL;
    
    node->id = id;
    node->visited = 0;
    node->neighbor_count = 0;
    node->capacity = 4;
    node->neighbors = malloc(sizeof(struct Node*) * node->capacity);
    if (node->neighbors == NULL) {
        free(node);
        return NULL;
    }
    return node;
}

int add_node(struct Graph* g, int id) {
    if (g->node_count >= MAX_NODES || id < 0 || id >= MAX_NODES) return 0;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == id) return 0;
    }
    
    struct Node* node = create_node(id);
    if (node == NULL) return 0;
    
    g->nodes[g->node_count] = node;
    g->node_count++;
    return 1;
}

int add_edge(struct Graph* g, int from, int to) {
    if (from < 0 || from >= MAX_NODES || to < 0 || to >= MAX_NODES) return 0;
    
    struct Node* from_node = NULL;
    struct Node* to_node = NULL;
    
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == from) from_node = g->nodes[i];
        if (g->nodes[i]->id == to) to_node = g->nodes[i];
    }
    
    if (from_node == NULL || to_node == NULL) return 0;
    
    if (from_node->neighbor_count >= from_node->capacity) {
        int new_capacity = from_node->capacity * 2;
        if (new_capacity > MAX_NODES) new_capacity = MAX_NODES;
        
        struct Node** new_neighbors = realloc(from_node->neighbors, sizeof(struct Node*) * new_capacity);
        if (new_neighbors == NULL) return 0;
        
        from_node->neighbors = new_neighbors;
        from_node->capacity = new_capacity;
    }
    
    for (int i = 0; i < from_node->neighbor_count; i++) {
        if (*(from_node->neighbors + i) == to_node) return 0;
    }
    
    *(from_node->neighbors + from_node->neighbor_count) = to_node;
    from_node->neighbor_count++;
    return 1;
}

void reset_visited(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        g->nodes[i]->visited = 0;
    }
}

void dfs(struct Node* node) {
    if (node == NULL || node->visited) return;
    
    node->visited = 1;
    printf("%d ", node->id);
    
    for (int i = 0; i < node->neighbor_count; i++) {
        struct Node* neighbor = *(node->neighbors + i);
        if (!neighbor->visited) {
            dfs(neighbor);
        }
    }
}

void bfs(struct Graph* g, int start_id) {
    if (g->node_count == 0) return;
    
    reset_visited(g);
    
    struct Node* queue[MAX_NODES];
    int front = 0, rear = 0;
    
    struct Node* start = NULL;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i]->id == start_id) {
            start = g->nodes[i];
            break;
        }
    }
    
    if (start == NULL) return;
    
    *(queue + rear) = start;
    rear = (rear + 1) % MAX_NODES;
    start->visited = 1;
    
    while (front != rear) {
        struct Node* current = *(queue + front);
        front = (front + 1) % MAX_NODES;
        
        printf("%d ", current->id);
        
        for (int i = 0; i < current->neighbor_count; i++) {
            struct Node* neighbor = *(current->neighbors + i);
            if (!neighbor->visited) {
                neighbor->visited = 1;
                if ((rear + 1) % MAX_NODES != front) {
                    *(queue + rear) = neighbor;
                    rear = (rear + 1) % MAX_NODES;
                }
            }
        }
    }
}

void free_graph(struct Graph* g) {
    for (int i = 0; i < g->node_count; i++) {
        free(g->nodes[i]->neighbors);
        free(g->nodes[i]);
    }
}

int main() {
    struct Graph g;
    init_graph(&g);
    
    printf