//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

typedef struct {
    int adjacency[MAX_NODES][MAX_NODES];
    int node_count;
} Graph;

typedef void (*GraphOperation)(Graph*, int, int);

void initialize_graph(Graph* graph, int node_count) {
    if (graph == NULL || node_count <= 0 || node_count > MAX_NODES) {
        return;
    }
    graph->node_count = node_count;
    for (int i = 0; i < node_count; i++) {
        for (int j = 0; j < node_count; j++) {
            graph->adjacency[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void add_edge(Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || from >= graph->node_count || 
        to < 0 || to >= graph->node_count || weight < 0) {
        return;
    }
    graph->adjacency[from][to] = weight;
}

void dijkstra_operation(Graph* graph, int start, int end) {
    if (graph == NULL || start < 0 || start >= graph->node_count || 
        end < 0 || end >= graph->node_count) {
        printf("Invalid parameters for Dijkstra\n");
        return;
    }
    
    int distances[MAX_NODES];
    int visited[MAX_NODES];
    
    for (int i = 0; i < graph->node_count; i++) {
        distances[i] = INF;
        visited[i] = 0;
    }
    distances[start] = 0;
    
    for (int count = 0; count < graph->node_count - 1; count++) {
        int min_distance = INF;
        int min_index = -1;
        
        for (int v = 0; v < graph->node_count; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_index = v;
            }
        }
        
        if (min_index == -1) break;
        visited[min_index] = 1;
        
        for (int v = 0; v < graph->node_count; v++) {
            if (!visited[v] && graph->adjacency[min_index][v] != INF && 
                distances[min_index] != INF) {
                long new_dist = (long)distances[min_index] + graph->adjacency[min_index][v];
                if (new_dist < distances[v] && new_dist <= INT_MAX) {
                    distances[v] = (int)new_dist;
                }
            }
        }
    }
    
    printf("Shortest distance from %d to %d: ", start, end);
    if (distances[end] == INF) {
        printf("No path exists\n");
    } else {
        printf("%d\n", distances[end]);
    }
}

void bfs_operation(Graph* graph, int start, int end) {
    if (graph == NULL || start < 0 || start >= graph->node_count || 
        end < 0 || end >= graph->node_count) {
        printf("Invalid parameters for BFS\n");
        return;
    }
    
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    printf("BFS traversal from %d: ", start);
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        if (current == end) {
            printf("(found target)\n");
            return;
        }
        
        for (int i = 0; i < graph->node_count; i++) {
            if (graph->adjacency[current][i] != INF && graph->adjacency[current][i] > 0 && !visited[i]) {
                if (rear < MAX_NODES) {
                    visited[i] = 1;
                    queue[rear++] = i;
                }
            }
        }
    }
    printf("(target not found)\n");
}

void execute_graph_operation(Graph* graph, GraphOperation operation, int start, int end) {
    if (graph == NULL || operation == NULL) {
        return;
    }
    operation(graph, start, end);
}

int main() {
    Graph graph;
    int node_count = 6;
    
    initialize_graph(&graph, node_count);
    
    add_edge(&graph, 0, 1, 4);
    add_edge(&graph, 0, 2, 2);
    add_edge(&graph, 1, 2, 1);
    add_edge(&graph, 1, 3, 5);
    add_edge(&graph, 2, 3, 8);
    add_edge(&graph, 2, 4, 10);
    add_edge(&graph, 3, 4, 2);
    add_edge(&graph, 3, 5, 6);
    add_edge(&graph, 4, 5, 3);
    
    printf("Graph Algorithms Demo\n");
    printf("====================\n");
    
    GraphOperation operations[] = {dijkstra_operation, bfs_operation};
    char* operation_names[] = {"Dijkstra", "BFS"};
    int num_operations = sizeof(operations