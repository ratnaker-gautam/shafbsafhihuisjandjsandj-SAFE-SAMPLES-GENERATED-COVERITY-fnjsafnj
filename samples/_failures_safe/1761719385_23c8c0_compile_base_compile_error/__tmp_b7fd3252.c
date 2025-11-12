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
    HeapNode* arr;
    int size;
    int capacity;
} MinHeap;

MinHeap* createHeap(int capacity) {
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

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->arr[parent].dist <= heap->arr[idx].dist) break;
        swapHeapNodes(&heap->arr[parent], &heap->arr[idx]);
        idx = parent;
    }
}

void heapifyDown(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist)
        smallest = left;
    if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist)
        smallest = right;
    
    if (smallest != idx) {
        swapHeapNodes(&heap->arr[idx], &heap->arr[smallest]);
        heapifyDown(heap, smallest);
    }
}

void pushHeap(MinHeap* heap, int node, int dist) {
    if (heap->size >= heap->capacity) return;
    heap->arr[heap->size].node = node;
    heap->arr[heap->size].dist = dist;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode popHeap(MinHeap* heap) {
    HeapNode empty = {-1, -1};
    if (heap->size <= 0) return empty;
    HeapNode root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return root;
}

int isEmptyHeap(MinHeap* heap) {
    return heap->size == 0;
}

void freeHeap(MinHeap* heap) {
    if (heap) {
        free(heap->arr);
        free(heap);
    }
}

int dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int src, int dest) {
    if (src < 0 || src >= n || dest < 0 || dest >= n) return -1;
    
    int dist[MAX_NODES];
    for (int i = 0; i < n; i++) dist[i] = INT_MAX;
    dist[src] = 0;
    
    MinHeap* heap = createHeap(MAX_NODES);
    if (!heap) return -1;
    pushHeap(heap, src, 0);
    
    while (!isEmptyHeap(heap)) {
        HeapNode current = popHeap(heap);
        int u = current.node;
        
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != 0) {
                int weight = graph[u][v];
                if (weight < 0) continue;
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pushHeap(heap, v, dist[v]);
                }
            }
        }
    }
    
    int result = dist[dest];
    freeHeap(heap);
    return (result == INT_MAX) ? -1 : result;
}

int main() {
    int graph[MAX_NODES][MAX_NODES] = {0};
    int n, m;
    
    printf("Enter number of nodes (1-100): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    
    printf("Enter number of edges: ");
    if (scanf("%d", &m) != 1 || m < 0 || m > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }
    
    printf("Enter edges (u v weight) for %d edges:\n", m);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            printf("Invalid edge input\n");
            return 1;
        }
        if (u < 0 || u >= n || v < 0 || v >= n) {
            printf("Invalid node index\n");
            return 1;
        }
        if (w < 0) {
            printf("Negative weights not allowed\n");