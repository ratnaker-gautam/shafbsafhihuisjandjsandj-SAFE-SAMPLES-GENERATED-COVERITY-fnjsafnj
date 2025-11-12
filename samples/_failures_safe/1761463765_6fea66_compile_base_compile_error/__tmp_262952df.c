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
    int node_count;
    int edge_count;
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

void heap_push(MinHeap *heap, int node, int distance) {
    if (heap->size >= MAX_NODES) return;
    
    int i = heap->size;
    heap->nodes[i].node = node;
    heap->nodes[i].distance = distance;
    heap->size++;
    
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->nodes[parent].distance <= heap->nodes[i].distance) break;
        heap_swap(&heap->nodes[parent], &heap->nodes[i]);
        i = parent;
    }
}

HeapNode heap_pop(MinHeap *heap) {
    HeapNode result = heap->nodes[0];
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        
        if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance) {
            smallest = right;
        }
        if (smallest == i) break;
        
        heap_swap(&heap->nodes[i], &heap->nodes[smallest]);
        i = smallest;
    }
    
    return result;
}

int heap_empty(MinHeap *heap) {
    return heap->size == 0;
}

void graph_init(Graph *graph) {
    graph->node_count = 0;
    graph->edge_count = 0;
}

int graph_add_edge(Graph *graph, int u, int v, int weight) {
    if (graph->edge_count >= MAX_EDGES) return 0;
    if (u < 0 || u >= MAX_NODES || v < 0 || v >= MAX_NODES) return 0;
    if (weight < 0) return 0;
    
    graph->edges[graph->edge_count].u = u;
    graph->edges[graph->edge_count].v = v;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return 1;
}

void dijkstra(Graph *graph, int start, int distances[]) {
    int visited[MAX_NODES] = {0};
    
    for (int i = 0; i < MAX_NODES; i++) {
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
        
        for (int i = 0; i < graph->edge_count; i++) {
            Edge edge = graph->edges[i];
            if (edge.u != u) continue;
            
            int v = edge.v;
            int new_dist = distances[u] + edge.weight;
            
            if (new_dist >= 0 && new_dist < distances[v]) {
                distances[v] = new_dist;
                heap_push(&heap, v, new_dist);
            }
        }
    }
}

int main() {
    Graph graph;
    graph_init(&graph);
    
    int node_count, edge_count;
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (scanf("%d", &node_count) != 1 || node_count < 1 || node_count > MAX_NODES) {
        printf("Invalid node count\n");
        return 1;
    }
    
    printf("Enter number of edges (0-%d): ", MAX_EDGES);
    if (scanf("%d", &edge_count) != 1 || edge_count < 0 || edge_count > MAX_EDGES) {
        printf("Invalid edge count\n");
        return 1;
    }
    
    graph.node_count = node_count;
    
    printf("Enter edges as 'u v weight' (nodes 0-%d):\n", node_count - 1);
    for (int i = 0; i < edge_count; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("Invalid edge format\n");
            return 1;
        }
        if (u < 0 || u >= node_count || v < 0 || v >= node_count) {