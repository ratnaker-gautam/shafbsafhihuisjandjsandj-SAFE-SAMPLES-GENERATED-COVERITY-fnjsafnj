//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define MAX_EDGES 500

typedef struct {
    int node, dist;
} HeapNode;

typedef struct {
    HeapNode* arr;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    if (capacity <= 0) return NULL;
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!heap) return NULL;
    heap->arr = (HeapNode*)malloc(capacity * sizeof(HeapNode));
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

void minHeapify(MinHeap* heap, int idx) {
    if (!heap || idx < 0 || idx >= heap->size) return;
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->arr[left].dist < heap->arr[smallest].dist)
        smallest = left;
    if (right < heap->size && heap->arr[right].dist < heap->arr[smallest].dist)
        smallest = right;
    if (smallest != idx) {
        swapHeapNodes(&heap->arr[idx], &heap->arr[smallest]);
        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap) {
    HeapNode empty = {-1, INT_MAX};
    if (!heap || heap->size <= 0) return empty;
    if (heap->size == 1) {
        heap->size--;
        return heap->arr[0];
    }
    HeapNode root = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(MinHeap* heap, int node, int dist) {
    if (!heap) return;
    for (int i = 0; i < heap->size; i++) {
        if (heap->arr[i].node == node) {
            heap->arr[i].dist = dist;
            while (i && heap->arr[i].dist < heap->arr[(i - 1) / 2].dist) {
                swapHeapNodes(&heap->arr[i], &heap->arr[(i - 1) / 2]);
                i = (i - 1) / 2;
            }
            break;
        }
    }
}

int isInHeap(MinHeap* heap, int node) {
    if (!heap) return 0;
    for (int i = 0; i < heap->size; i++) {
        if (heap->arr[i].node == node)
            return 1;
    }
    return 0;
}

void freeMinHeap(MinHeap* heap) {
    if (heap) {
        free(heap->arr);
        free(heap);
    }
}

int dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int src, int dest) {
    if (n <= 0 || n > MAX_NODES || src < 0 || src >= n || dest < 0 || dest >= n)
        return -1;
    int dist[MAX_NODES];
    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;
    MinHeap* heap = createMinHeap(n);
    if (!heap) return -1;
    for (int i = 0; i < n; i++) {
        heap->arr[heap->size].node = i;
        heap->arr[heap->size].dist = dist[i];
        heap->size++;
    }
    while (heap->size > 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;
        if (u == dest) {
            freeMinHeap(heap);
            return dist[u];
        }
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != 0 && dist[u] != INT_MAX) {
                if (graph[u][v] < 0) continue;
                long long alt = (long long)dist[u] + graph[u][v];
                if (alt > INT_MAX) continue;
                if (alt < dist[v]) {
                    dist[v] = (int)alt;
                    decreaseKey(heap, v, dist[v]);
                }
            }
        }
    }
    freeMinHeap(heap);
    return -1;
}

int main() {
    int graph[MAX_NODES][MAX_NODES];
    int n, m;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }
    printf("Enter number of edges: ");
    if (scanf("%d", &m) != 1 || m < 0 || m > MAX_EDGES) {
        printf