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
    HeapNode* arr;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->arr = malloc(capacity * sizeof(HeapNode));
    if (!heap->arr) {
        free(heap);
        return NULL;
    }
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
        if (heap->arr[parent].dist <= heap->arr[idx].dist) break;
        swap_nodes(&heap->arr[parent], &heap->arr[idx]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist) {
            smallest = left;
        }
        if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist) {
            smallest = right;
        }
        if (smallest == idx) break;
        swap_nodes(&heap->arr[idx], &heap->arr[smallest]);
        idx = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int dist) {
    if (!heap || heap->size >= heap->capacity) return;
    heap->arr[heap->size].node = node;
    heap->arr[heap->size].dist = dist;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (!heap || heap->size <= 0) return empty;
    HeapNode root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return root;
}

int is_heap_empty(MinHeap* heap) {
    return !heap || heap->size == 0;
}

void free_heap(MinHeap* heap) {
    if (!heap) return;
    free(heap->arr);
    free(heap);
}

int validate_graph_params(int num_nodes, int num_edges) {
    if (num_nodes <= 0 || num_nodes > MAX_NODES) return 0;
    if (num_edges < 0 || num_edges > MAX_EDGES) return 0;
    if (num_edges > num_nodes * (num_nodes - 1) / 2) return 0;
    return 1;
}

int read_int(const char* prompt, int min_val, int max_val) {
    int value;
    printf("%s", prompt);
    if (scanf("%d", &value) != 1) {
        fprintf(stderr, "Invalid input\n");
        exit(1);
    }
    if (value < min_val || value > max_val) {
        fprintf(stderr, "Value out of range\n");
        exit(1);
    }
    return value;
}

void dijkstra(int num_nodes, Edge edges[], int num_edges, int start_node) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < num_nodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }
    dist[start_node] = 0;
    
    MinHeap* heap = create_heap(num_nodes);
    if (!heap) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    heap_push(heap, start_node, 0);
    
    while (!is_heap_empty(heap)) {
        HeapNode current = heap_pop(heap);
        int u = current.node;
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int i = 0; i < num_edges; i++) {
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
    
    printf("Shortest distances from node %d:\n", start_node);
    for (int i = 0; i < num_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("Node %d: unreachable\n", i);
        } else {
            printf("Node %d: