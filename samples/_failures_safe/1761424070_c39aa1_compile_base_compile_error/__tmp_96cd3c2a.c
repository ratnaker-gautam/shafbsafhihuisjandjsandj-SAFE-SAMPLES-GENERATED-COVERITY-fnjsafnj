//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

typedef struct {
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* elements;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    if (capacity <= 0) return NULL;
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->elements = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    if (!heap->elements) {
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

void minHeapify(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->elements[left].dist < heap->elements[smallest].dist)
        smallest = left;
    if (right < heap->size && heap->elements[right].dist < heap->elements[smallest].dist)
        smallest = right;
    if (smallest != idx) {
        swapHeapNodes(&heap->elements[idx], &heap->elements[smallest]);
        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (!heap || heap->size <= 0) return empty;
    if (heap->size == 1) {
        heap->size--;
        return heap->elements[0];
    }
    HeapNode root = heap->elements[0];
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(MinHeap* heap, int node, int dist) {
    if (!heap) return;
    int i;
    for (i = 0; i < heap->size; i++) {
        if (heap->elements[i].node == node) {
            heap->elements[i].dist = dist;
            break;
        }
    }
    while (i > 0 && heap->elements[i].dist < heap->elements[(i - 1) / 2].dist) {
        swapHeapNodes(&heap->elements[i], &heap->elements[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInHeap(MinHeap* heap, int node) {
    if (!heap) return 0;
    for (int i = 0; i < heap->size; i++) {
        if (heap->elements[i].node == node)
            return 1;
    }
    return 0;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int src, int dist[]) {
    if (n <= 0 || n > MAX_NODES || src < 0 || src >= n || !dist) return;
    
    MinHeap* heap = createMinHeap(n);
    if (!heap) return;
    
    for (int v = 0; v < n; v++) {
        dist[v] = INT_MAX;
        if (v < heap->capacity) {
            heap->elements[v].node = v;
            heap->elements[v].dist = dist[v];
        }
    }
    heap->size = n;
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);
    
    while (heap->size > 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;
        if (u == -1) break;
        
        for (int v = 0; v < n; v++) {
            if (graph[u][v] && dist[u] != INT_MAX) {
                long new_dist = (long)dist[u] + graph[u][v];
                if (new_dist > INT_MAX) continue;
                if (new_dist < dist[v]) {
                    dist[v] = (int)new_dist;
                    if (isInHeap(heap, v)) {
                        decreaseKey(heap, v, dist[v]);
                    }
                }
            }
        }
    }
    free(heap->elements);
    free(heap);
}

int main() {
    int graph[MAX_NODES][MAX_NODES] = {0};
    int n, m;
    printf("Enter number of nodes and edges: ");
    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || n > MAX_NODES || m < 0 || m > MAX_EDGES) {
        printf("Invalid input\n");
        return 1;
    }
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= n || v <