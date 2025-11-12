//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 1000

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    int edge_weights[MAX_NODES][MAX_NODES];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
            g->edge_weights[i][j] = 0;
        }
    }
}

int add_node(struct Graph *g) {
    if (g->num_nodes >= MAX_NODES) {
        return -1;
    }
    return g->num_nodes++;
}

int add_edge(struct Graph *g, int from, int to, int weight) {
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes) {
        return -1;
    }
    if (g->num_edges >= MAX_EDGES) {
        return -1;
    }
    if (g->adjacency[from][to] == 1) {
        return -1;
    }
    g->adjacency[from][to] = 1;
    g->edge_weights[from][to] = weight;
    g->num_edges++;
    return 0;
}

void depth_first_search(struct Graph *g, int start, int visited[]) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    visited[start] = 1;
    printf("%d ", start);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[start][i] == 1 && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void breadth_first_search(struct Graph *g, int start) {
    if (start < 0 || start >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    
    visited[start] = 1;
    queue[rear++] = start;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);
        
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] == 1 && !visited[i]) {
                visited[i] = 1;
                if (rear < MAX_NODES) {
                    queue[rear++] = i;
                }
            }
        }
    }
}

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

int main() {
    struct Graph graph;
    initialize_graph(&graph);
    
    printf("Graph Algorithms Demo\n");
    printf("====================\n\n");
    
    int num_nodes = read_int("Enter number of nodes (1-100): ", 1, MAX_NODES);
    if (num_nodes == -1) {
        return 1;
    }
    
    for (int i = 0; i < num_nodes; i++) {
        add_node(&graph);
    }
    
    printf("Added %d nodes to the graph.\n\n", num_nodes);
    
    printf("Adding edges (enter -1 to stop):\n");
    while (1) {
        int from = read_int("Enter source node (-1 to stop): ", -1, num_nodes - 1);
        if (from == -1) {
            break;
        }
        
        int to = read_int("Enter destination node: ", 0, num_nodes - 1);
        int weight = read_int("Enter edge weight (1-100): ", 1, 100);
        
        if (add_edge(&graph, from, to, weight) == -1) {
            printf("Failed to add edge. Edge may already exist or graph is full.\n");
        } else {
            printf("Added edge %d -> %d with weight %d\n", from, to, weight);
        }
    }
    
    printf("\nGraph operations:\n");
    printf("1. Depth First Search\n");
    printf("2. Breadth First Search\n");
    printf("3. Exit\n");
    
    while (1) {
        int choice = read_int("\nEnter choice (1-3): ", 1, 3);
        
        if (choice == 3) {
            break;
        }
        
        int start_node = read_int("Enter starting node: ", 0, num_nodes - 1);
        
        if (choice == 1) {