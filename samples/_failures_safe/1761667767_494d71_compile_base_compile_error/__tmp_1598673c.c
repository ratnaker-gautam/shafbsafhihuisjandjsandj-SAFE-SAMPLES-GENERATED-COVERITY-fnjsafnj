//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000
#define MAX_LINE 256

typedef struct {
    int from;
    int to;
    int weight;
} Edge;

typedef struct {
    int node_count;
    int edge_count;
    Edge edges[MAX_EDGES];
} Graph;

int read_graph(Graph *graph) {
    char line[MAX_LINE];
    int node_count = 0;
    
    printf("Enter number of nodes (1-%d): ", MAX_NODES);
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    if (sscanf(line, "%d", &node_count) != 1) return 0;
    if (node_count < 1 || node_count > MAX_NODES) return 0;
    
    graph->node_count = node_count;
    graph->edge_count = 0;
    
    printf("Enter edges (from to weight, -1 -1 -1 to stop):\n");
    while (graph->edge_count < MAX_EDGES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        int from, to, weight;
        if (sscanf(line, "%d %d %d", &from, &to, &weight) != 3) continue;
        if (from == -1 && to == -1 && weight == -1) break;
        
        if (from < 0 || from >= node_count || to < 0 || to >= node_count) {
            printf("Invalid node indices\n");
            continue;
        }
        
        if (weight < 0) {
            printf("Negative weights not allowed\n");
            continue;
        }
        
        graph->edges[graph->edge_count].from = from;
        graph->edges[graph->edge_count].to = to;
        graph->edges[graph->edge_count].weight = weight;
        graph->edge_count++;
    }
    
    return 1;
}

int compare_edges(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;
    return ea->weight - eb->weight;
}

int find_parent(int parent[], int node) {
    if (parent[node] != node) {
        parent[node] = find_parent(parent, parent[node]);
    }
    return parent[node];
}

void kruskal_mst(Graph *graph) {
    if (graph->edge_count == 0) {
        printf("No edges in graph\n");
        return;
    }
    
    Edge result[MAX_NODES];
    int result_count = 0;
    
    qsort(graph->edges, graph->edge_count, sizeof(Edge), compare_edges);
    
    int parent[MAX_NODES];
    for (int i = 0; i < graph->node_count; i++) {
        parent[i] = i;
    }
    
    int total_weight = 0;
    
    for (int i = 0; i < graph->edge_count && result_count < graph->node_count - 1; i++) {
        Edge current = graph->edges[i];
        int root_from = find_parent(parent, current.from);
        int root_to = find_parent(parent, current.to);
        
        if (root_from != root_to) {
            result[result_count++] = current;
            total_weight += current.weight;
            parent[root_from] = root_to;
        }
    }
    
    if (result_count != graph->node_count - 1) {
        printf("Graph is not connected\n");
        return;
    }
    
    printf("Minimum Spanning Tree edges:\n");
    for (int i = 0; i < result_count; i++) {
        printf("%d - %d (weight: %d)\n", result[i].from, result[i].to, result[i].weight);
    }
    printf("Total weight: %d\n", total_weight);
}

void bfs_shortest_path(Graph *graph, int start) {
    if (start < 0 || start >= graph->node_count) {
        printf("Invalid start node\n");
        return;
    }
    
    int visited[MAX_NODES];
    int distance[MAX_NODES];
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    for (int i = 0; i < graph->node_count; i++) {
        visited[i] = 0;
        distance[i] = -1;
    }
    
    visited[start] = 1;
    distance[start] = 0;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        
        for (int i = 0; i < graph->edge_count; i++) {
            if (graph->edges[i].from == current && !visited[graph->edges[i].to]) {
                visited[graph->edges[i].to] = 1;
                distance[graph->edges[i].to] = distance[current] + 1;
                queue[rear++] = graph->edges[i].to;
            }
            if (graph->edges[i].to == current && !visited[graph->edges[i].from]) {
                visited[graph->edges[i].from] = 1;
                distance[graph->edges[i].from] = distance[current] + 1;
                queue[rear++] = graph->edges[i].from;
            }
        }
    }
    
    printf("Shortest distances from node %d