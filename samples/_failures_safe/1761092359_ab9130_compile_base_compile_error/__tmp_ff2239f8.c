//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode nodes[MAX_NODES];
    int size;
} MinHeap;

void init_graph(Graph* g, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) {
        node_count = MAX_NODES;
    }
    g->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            g->edges[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
}

void add_edge(Graph* g, int from, int to, int weight) {
    if (from < 0 || from >= g->node_count || to < 0 || to >= g->node_count) {
        return;
    }
    if (weight < 0) {
        return;
    }
    g->edges[from][to] = weight;
}

void heap_init(MinHeap* heap) {
    heap->size = 0;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= MAX_NODES) {
        return;
    }
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    int current = heap->size;
    heap->size++;
    
    while (current > 0) {
        int parent = (current - 1) / 2;
        if (heap->nodes[parent].distance <= heap->nodes[current].distance) {
            break;
        }
        heap_swap(&heap->nodes[parent], &heap->nodes[current]);
        current = parent;
    }
}

int heap_pop(MinHeap* heap) {
    if (heap->size == 0) {
        return -1;
    }
    int result = heap->nodes[0].node;
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    
    int current = 0;
    while (1) {
        int left = 2 * current + 1;
        int right = 2 * current + 2;
        int smallest = current;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == current) {
            break;
        }
        heap_swap(&heap->nodes[current], &heap->nodes[smallest]);
        current = smallest;
    }
    return result;
}

int heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(Graph* g, int start, int distances[]) {
    if (start < 0 || start >= g->node_count) {
        return;
    }
    
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap;
    heap_init(&heap);
    heap_push(&heap, start, 0);
    
    while (!heap_empty(&heap)) {
        int current = heap_pop(&heap);
        if (visited[current]) {
            continue;
        }
        visited[current] = 1;
        
        for (int neighbor = 0; neighbor < g->node_count; neighbor++) {
            if (g->edges[current][neighbor] != INT_MAX && !visited[neighbor]) {
                int new_dist;
                if (distances[current] > INT_MAX - g->edges[current][neighbor]) {
                    continue;
                }
                new_dist = distances[current] + g->edges[current][neighbor];
                if (new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                    heap_push(&heap, neighbor, new_dist);
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    int node_count, edge_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    init_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > node_count * node_count) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges (from to weight):\n");
    for (int i =