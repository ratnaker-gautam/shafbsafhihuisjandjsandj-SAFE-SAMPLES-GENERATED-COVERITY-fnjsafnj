//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

typedef struct {
    int edges[MAX_NODES][MAX_NODES];
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

void heap_init(MinHeap* heap) {
    heap->size = 0;
}

void heap_swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_up(MinHeap* heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->nodes[index].distance >= heap->nodes[parent].distance) break;
        heap_swap(&heap->nodes[index], &heap->nodes[parent]);
        index = parent;
    }
}

void heap_down(MinHeap* heap, int index) {
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == index) break;
        heap_swap(&heap->nodes[index], &heap->nodes[smallest]);
        index = smallest;
    }
}

void heap_push(MinHeap* heap, int node, int distance) {
    if (heap->size >= MAX_NODES) return;
    heap->nodes[heap->size].node = node;
    heap->nodes[heap->size].distance = distance;
    heap_up(heap, heap->size);
    heap->size++;
}

HeapNode heap_pop(MinHeap* heap) {
    HeapNode result = heap->nodes[0];
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    heap_down(heap, 0);
    return result;
}

int heap_empty(MinHeap* heap) {
    return heap->size == 0;
}

void graph_init(Graph* graph, int node_count) {
    if (node_count <= 0 || node_count > MAX_NODES) node_count = MAX_NODES;
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            graph->edges[i][j] = (i == j) ? 0 : INT_MAX;
        }
    }
}

void graph_add_edge(Graph* graph, int from, int to, int weight) {
    if (from < 0 || from >= graph->node_count) return;
    if (to < 0 || to >= graph->node_count) return;
    if (weight < 0) return;
    graph->edges[from][to] = weight;
}

void dijkstra(Graph* graph, int start, int distances[]) {
    int visited[MAX_NODES] = {0};
    
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
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        for (int v = 0; v < graph->node_count; v++) {
            if (graph->edges[u][v] != INT_MAX && !visited[v]) {
                int alt;
                if (distances[u] == INT_MAX) {
                    alt = INT_MAX;
                } else {
                    alt = distances[u] + graph->edges[u][v];
                    if (alt < 0) alt = INT_MAX;
                }
                
                if (alt < distances[v]) {
                    distances[v] = alt;
                    heap_push(&heap, v, alt);
                }
            }
        }
    }
}

int main() {
    Graph graph;
    int node_count;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    graph_init(&graph, node_count);
    
    int edge_count;
    printf("Enter number of edges: ");
    if (scanf("%d", &edge_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (edge_count < 0 || edge_count > node_count * node_count) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    printf("Enter edges as 'from to weight' (0-indexed):\n");
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        if