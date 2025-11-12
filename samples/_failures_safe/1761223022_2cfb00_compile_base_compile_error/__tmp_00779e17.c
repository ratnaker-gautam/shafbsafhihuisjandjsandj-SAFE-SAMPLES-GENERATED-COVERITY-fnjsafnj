//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* heap;
    int size;
    int capacity;
} MinHeap;

typedef struct {
    Edge edges[MAX_EDGES];
    int edge_count;
    int node_count;
} Graph;

MinHeap* create_heap(int capacity) {
    if (capacity <= 0) return NULL;
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->heap = malloc(capacity * sizeof(HeapNode));
    if (!heap->heap) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    if (!heap || idx <= 0) return;
    int parent = (idx - 1) / 2;
    if (heap->heap[parent].dist > heap->heap[idx].dist) {
        heap_swap(&heap->heap[parent], &heap->heap[idx]);
        heapify_up(heap, parent);
    }
}

void heapify_down(MinHeap* heap, int idx) {
    if (!heap) return;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;
    
    if (left < heap->size && heap->heap[left].dist < heap->heap[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->heap[right].dist < heap->heap[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        heap_swap(&heap->heap[idx], &heap->heap[smallest]);
        heapify_down(heap, smallest);
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (!heap || heap->size >= heap->capacity) return;
    heap->heap[heap->size].node = node;
    heap->heap[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (!heap || heap->size <= 0) return empty;
    HeapNode root = heap->heap[0];
    heap->heap[0] = heap->heap[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

void init_graph(Graph* graph, int nodes) {
    if (!graph) return;
    if (nodes <= 0 || nodes > MAX_NODES) nodes = MAX_NODES;
    graph->node_count = nodes;
    graph->edge_count = 0;
}

int add_edge(Graph* graph, int u, int v, int weight) {
    if (!graph) return 0;
    if (u < 0 || u >= graph->node_count || v < 0 || v >= graph->node_count) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->edge_count].u = u;
    graph->edges[graph->edge_count].v = v;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(Graph* graph, int start, int dist[]) {
    if (!graph || !dist || start < 0 || start >= graph->node_count) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    
    MinHeap* heap = create_heap(graph->node_count);
    if (!heap) return;
    
    heap_push(heap, start, 0);
    
    while (heap->size > 0) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].u == u) {
                int v = graph->edges[i].v;
                int weight = graph->edges[i].weight;
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    heap_push(heap, v, dist[v]);
                }
            }
        }
    }
    
    free(heap->heap);
    free(heap);
}

int main() {
    Graph graph;
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1 || nodes < 1 || nodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    init_graph(&graph, nodes);