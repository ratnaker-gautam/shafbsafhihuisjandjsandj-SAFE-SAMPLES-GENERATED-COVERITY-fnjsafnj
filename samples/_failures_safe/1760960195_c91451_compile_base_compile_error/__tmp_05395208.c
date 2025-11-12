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

void init_graph(Graph* g, int n) {
    if (n <= 0 || n > MAX_NODES) n = MAX_NODES;
    g->node_count = n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->edges[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
}

void add_edge(Graph* g, int u, int v, int weight) {
    if (u < 0 || u >= g->node_count || v < 0 || v >= g->node_count) return;
    if (weight < 0) return;
    g->edges[u][v] = weight;
    g->edges[v][u] = weight;
}

void heapify_up(MinHeap* heap, int index) {
    if (index <= 0) return;
    int parent = (index - 1) / 2;
    if (heap->nodes[index].distance < heap->nodes[parent].distance) {
        HeapNode temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[parent];
        heap->nodes[parent] = temp;
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    
    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
        smallest = right;
    }
    if (smallest != index) {
        HeapNode temp = heap->nodes[index];
        heap->nodes[index] = heap->nodes[smallest];
        heap->nodes[smallest] = temp;
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= MAX_NODES) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (heap->size <= 0) return empty;
    HeapNode result = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return result;
}

int heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void dijkstra(Graph* g, int start, int distances[]) {
    if (start < 0 || start >= g->node_count) return;
    
    for (int i = 0; i < g->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap = {0};
    heap_push(&heap, start, 0);
    
    while (!heap_empty(&heap)) {
        HeapNode current = heap_pop(&heap);
        int u = current.node;
        
        for (int v = 0; v < g->node_count; v++) {
            if (g->edges[u][v] != INT_MAX && g->edges[u][v] >= 0) {
                int alt;
                if (distances[u] == INT_MAX) {
                    alt = INT_MAX;
                } else {
                    alt = distances[u] + g->edges[u][v];
                    if (alt < 0) alt = INT_MAX;
                }
                
                if (alt < distances[v]) {
                    distances[v] = alt;
                    heap_push(&heap, v, alt);
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
    if (node_count <= 0 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    if (edge_count < 0 || edge_count > node_count * (node_count - 1) / 2) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edge_count; i++) {
        int