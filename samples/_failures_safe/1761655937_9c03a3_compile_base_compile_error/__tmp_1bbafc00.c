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
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->data = malloc(capacity * sizeof(HeapNode));
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
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[parent].dist <= heap->data[idx].dist) break;
        heap_swap(&heap->data[parent], &heap->data[idx]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->data[left].dist < heap->data[smallest].dist) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right].dist < heap->data[smallest].dist) {
            smallest = right;
        }
        if (smallest == idx) break;
        heap_swap(&heap->data[idx], &heap->data[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->data[heap->size].node = node;
    heap->data[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode result = heap->data[0];
    heap->size--;
    heap->data[0] = heap->data[heap->size];
    heapify_down(heap, 0);
    return result;
}

int is_heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void free_heap(MinHeap* heap) {
    free(heap->data);
    free(heap);
}

int validate_graph_input(int nodes, int edges) {
    if (nodes <= 0 || nodes > MAX_NODES) return 0;
    if (edges < 0 || edges > MAX_EDGES) return 0;
    if (edges > nodes * (nodes - 1) / 2) return 0;
    return 1;
}

void dijkstra(int nodes, Edge* edges, int edge_count, int start) {
    if (start < 0 || start >= nodes) return;
    
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    MinHeap* heap = create_heap(nodes);
    heap_push(heap, start, 0);
    
    while (!is_heap_empty(heap)) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int i = 0; i < edge_count; i++) {
            if (edges[i].u == u) {
                int v = edges[i].v;
                int weight = edges[i].weight;
                if (weight < 0) continue;
                
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    heap_push(heap, v, dist[v]);
                }
            }
        }
    }
    
    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: %d\n", i, dist[i]);
        }
    }
    
    free_heap(heap);
}

int main() {
    int nodes, edges;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &nodes) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edges) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_graph_input(nodes, edges)) {
        printf("Invalid graph parameters\n");
        return 1;
    }
    
    Edge edge_list[MAX_EDGES];
    
    printf("Enter edges as 'u v weight' (nodes 0-indexed):\n");
    for