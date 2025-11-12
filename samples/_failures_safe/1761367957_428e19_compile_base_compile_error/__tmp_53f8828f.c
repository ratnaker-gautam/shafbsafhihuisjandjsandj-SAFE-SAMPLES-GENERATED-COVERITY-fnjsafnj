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
    Edge edges[MAX_EDGES];
    int edge_count;
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

void heap_init(MinHeap *heap) {
    heap->size = 0;
}

void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_up(MinHeap *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->nodes[parent].distance <= heap->nodes[idx].distance) break;
        heap_swap(&heap->nodes[parent], &heap->nodes[idx]);
        idx = parent;
    }
}

void heap_down(MinHeap *heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == idx) break;
        heap_swap(&heap->nodes[idx], &heap->nodes[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap *heap, int node, int distance) {
    if (heap->size >= MAX_NODES) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heap_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap *heap) {
    HeapNode result = heap->nodes[0];
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    heap_down(heap, 0);
    return result;
}

int heap_empty(MinHeap *heap) {
    return heap->size == 0;
}

void graph_init(Graph *graph, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) node_count = MAX_NODES;
    graph->node_count = node_count;
    graph->edge_count = 0;
}

int graph_add_edge(Graph *graph, int u, int v, int weight) {
    if (u < 0 || u >= graph->node_count) return 0;
    if (v < 0 || v >= graph->node_count) return 0;
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->edge_count].u = u;
    graph->edges[graph->edge_count].v = v;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(Graph *graph, int start, int distances[]) {
    if (start < 0 || start >= graph->node_count) return;
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INT_MAX;
    }
    distances[start] = 0;
    
    MinHeap heap;
    heap_init(&heap);
    heap_push(&heap, start, 0);
    
    while (!heap_empty(&heap)) {
        HeapNode current = heap_pop(&heap);
        int u = current.node;
        
        if (current.distance > distances[u]) continue;
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].u == u) {
                int v = graph->edges[i].v;
                int weight = graph->edges[i].weight;
                int new_dist = distances[u] + weight;
                
                if (new_dist >= 0 && new_dist < distances[v]) {
                    distances[v] = new_dist;
                    heap_push(&heap, v, new_dist);
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int distances[MAX_NODES];
    
    graph_init(&graph, 6);
    
    graph_add_edge(&graph, 0, 1, 4);
    graph_add_edge(&graph, 0, 2, 2);
    graph_add_edge(&graph, 1, 2, 1);
    graph_add_edge(&graph, 1, 3, 5);
    graph_add_edge(&graph, 2, 3, 8);
    graph_add_edge(&graph, 2, 4, 10);
    graph_add_edge(&graph, 3, 4, 2);
    graph_add_edge(&graph, 3, 5, 6);
    graph_add_edge(&graph, 4, 5, 3);
    
    int start_node;
    printf("Enter starting node (0-5): ");
    if (scanf("%d", &start_node