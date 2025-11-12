//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    int node, dist;
} HeapNode;

typedef struct {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    Edge edges[MAX_EDGES];
    int edge_count;
    int node_count;
} Graph;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->nodes = malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap_nodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->nodes[idx].dist >= heap->nodes[parent].dist) break;
        swap_nodes(&heap->nodes[idx], &heap->nodes[parent]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swap_nodes(&heap->nodes[idx], &heap->nodes[smallest]);
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    if (heap->size <= 0) {
        HeapNode empty = {-1, -1};
        return empty;
    }
    HeapNode root = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

void init_graph(Graph* graph, int nodes) {
    if (nodes <= 0 || nodes > MAX_NODES) nodes = MAX_NODES;
    graph->node_count = nodes;
    graph->edge_count = 0;
}

int add_edge(Graph* graph, int u, int v, int weight) {
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (u < 0 || u >= graph->node_count) return 0;
    if (v < 0 || v >= graph->node_count) return 0;
    graph->edges[graph->edge_count].u = u;
    graph->edges[graph->edge_count].v = v;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(Graph* graph, int start, int distances[]) {
    if (start < 0 || start >= graph->node_count) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap* heap = create_heap(graph->node_count);
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        for (int i = 0; i < graph->edge_count; i++) {
            Edge edge = graph->edges[i];
            if (edge.u == u) {
                int v = edge.v;
                int new_dist = distances[u] + edge.weight;
                if (new_dist >= 0 && new_dist < distances[v]) {
                    distances[v] = new_dist;
                    heap_push(heap, v, new_dist);
                }
            }
        }
    }
    
    free(heap->nodes);
    free(heap);
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    int node_count;
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&graph, node_count);
    
    printf("Enter number of edges: ");
    int edge_count;
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges as 'u v weight' (0-indexed):\n");
    for (int i = 0;