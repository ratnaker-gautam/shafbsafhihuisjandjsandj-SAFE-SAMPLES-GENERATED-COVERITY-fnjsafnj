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
    int distance;
} HeapNode;

typedef struct {
    HeapNode* elements;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->elements = malloc(capacity * sizeof(HeapNode));
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

    if (left < heap->size && heap->elements[left].distance < heap->elements[smallest].distance)
        smallest = left;
    if (right < heap->size && heap->elements[right].distance < heap->elements[smallest].distance)
        smallest = right;
    if (smallest != idx) {
        swapHeapNodes(&heap->elements[idx], &heap->elements[smallest]);
        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap* heap) {
    if (heap->size <= 0) {
        HeapNode empty = {-1, INT_MAX};
        return empty;
    }
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

void decreaseKey(MinHeap* heap, int node, int distance) {
    int i;
    for (i = 0; i < heap->size; i++) {
        if (heap->elements[i].node == node) {
            heap->elements[i].distance = distance;
            break;
        }
    }

    while (i != 0 && heap->elements[(i - 1) / 2].distance > heap->elements[i].distance) {
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

void dijkstra(int graph[MAX_NODES][MAX_NODES], int numNodes, int src) {
    if (numNodes <= 0 || numNodes > MAX_NODES || src < 0 || src >= numNodes) {
        printf("Invalid input parameters\n");
        return;
    }

    int dist[MAX_NODES];
    MinHeap* heap = createMinHeap(numNodes);

    for (int v = 0; v < numNodes; v++) {
        dist[v] = INT_MAX;
        heap->elements[v].node = v;
        heap->elements[v].distance = dist[v];
        heap->size++;
    }

    dist[src] = 0;
    decreaseKey(heap, src, dist[src]);

    while (heap->size != 0) {
        HeapNode minNode = extractMin(heap);
        int u = minNode.node;

        for (int v = 0; v < numNodes; v++) {
            if (graph[u][v] != 0 && isInHeap(heap, v)) {
                int weight = graph[u][v];
                if (dist[u] != INT_MAX && weight >= 0) {
                    long newDist = (long)dist[u] + weight;
                    if (newDist < INT_MAX && newDist < dist[v]) {
                        dist[v] = newDist;
                        decreaseKey(heap, v, dist[v]);
                    }
                }
            }
        }
    }

    printf("Vertex Distance from Source %d\n", src);
    for (int i = 0; i < numNodes; i++) {
        if (dist[i] == INT_MAX)
            printf("%d \t INF\n", i);
        else
            printf("%d \t %d\n", i, dist[i]);
    }

    free(heap->elements);
    free(heap);
}

int main() {
    int numNodes, numEdges;
    printf("Enter number of nodes (max %d): ", MAX_NODES);
    if (scanf("%d", &numNodes) != 1 || numNodes <= 0 || numNodes > MAX_NODES) {
        printf("Invalid number of nodes\n");
        return 1;
    }

    printf("Enter number of edges (max %d): ", MAX_EDGES);
    if (scanf("%d", &numEdges) != 1 || numEdges < 0 || numEdges > MAX_EDGES) {
        printf("Invalid number of edges\n");
        return 1;
    }

    int graph[MAX_NODES][MAX_NODES];
    for (int i = 0; i <