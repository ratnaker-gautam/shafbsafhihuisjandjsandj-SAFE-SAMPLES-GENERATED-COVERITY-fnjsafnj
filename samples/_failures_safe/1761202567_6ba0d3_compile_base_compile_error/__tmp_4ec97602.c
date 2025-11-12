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
    int node;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* elements;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
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
    if (heap->size <= 0)
        return (HeapNode){-1, INT_MAX};
    HeapNode root = heap->elements[0];
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return root;
}

void decreaseKey(MinHeap* heap, int node, int dist) {
    int i;
    for (i = 0; i < heap->size; i++) {
        if (heap->elements[i].node == node) {
            heap->elements[i].dist = dist;
            break;
        }
    }
    while (i && heap->elements[i].dist < heap->elements[(i - 1) / 2].dist) {
        swapHeapNodes(&heap->elements[i], &heap->elements[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInHeap(MinHeap* heap, int node) {
    for (int i = 0; i < heap->size; i++) {
        if (heap->elements[i].node == node)
            return 1;
    }
    return 0;
}

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int src, int dist[MAX_NODES]) {
    MinHeap* heap = createMinHeap(n);
    if (!heap) return;
    for (int v = 0; v < n; v++) {
        dist[v] = INT_MAX;
        heap->elements[heap->size].node = v;
        heap->elements[heap->size].dist = dist[v];
        heap->size++;
    }
    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);
    while (heap->size > 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;
        for (int v = 0; v < n; v++) {
            if (graph[u][v] && dist[u] != INT_MAX) {
                long new_dist = (long)dist[u] + graph[u][v];
                if (new_dist < dist[v] && new_dist < INT_MAX) {
                    dist[v] = (int)new_dist;
                    if (isInHeap(heap, v))
                        decreaseKey(heap, v, dist[v]);
                }
            }
        }
    }
    free(heap->elements);
    free(heap);
}

int main() {
    int n, m;
    printf("Enter number of nodes and edges: ");
    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || n > MAX_NODES || m < 0 || m > MAX_EDGES) {
        printf("Invalid input\n");
        return 1;
    }

    int graph[MAX_NODES][MAX_NODES] = {0};
    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3 || u < 0 || u >= n || v < 0 || v >= n || w < 0) {
            printf("Invalid edge\n");
            return 1;
        }
        graph[u][v] = w;
        graph[v][u] = w;
    }

    int src;
    printf("Enter source node: ");
    if (scanf("%d", &src) != 1 || src < 0 || src >= n) {
        printf("Invalid source node\n");
        return 1;
    }

    int dist[MAX_NODES];
    dijkstra(graph, n, src, dist);