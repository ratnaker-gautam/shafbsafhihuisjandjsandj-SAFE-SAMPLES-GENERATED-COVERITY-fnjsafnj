//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: graph_algorithms
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 500

struct Graph {
    int num_nodes;
    int num_edges;
    int adjacency[MAX_NODES][MAX_NODES];
    char node_names[MAX_NODES][32];
};

void initialize_graph(struct Graph *g) {
    g->num_nodes = 0;
    g->num_edges = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            g->adjacency[i][j] = 0;
        }
        g->node_names[i][0] = '\0';
    }
}

int find_node_index(struct Graph *g, const char *name) {
    for (int i = 0; i < g->num_nodes; i++) {
        if (strcmp(g->node_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

int add_node(struct Graph *g, const char *name) {
    if (g->num_nodes >= MAX_NODES || strlen(name) >= 32) {
        return -1;
    }
    if (find_node_index(g, name) != -1) {
        return -1;
    }
    strcpy(g->node_names[g->num_nodes], name);
    g->num_nodes++;
    return g->num_nodes - 1;
}

int add_edge(struct Graph *g, const char *from, const char *to, int weight) {
    if (g->num_edges >= MAX_EDGES) {
        return 0;
    }
    int from_idx = find_node_index(g, from);
    int to_idx = find_node_index(g, to);
    if (from_idx == -1 || to_idx == -1) {
        return 0;
    }
    g->adjacency[from_idx][to_idx] = weight;
    g->num_edges++;
    return 1;
}

void depth_first_search(struct Graph *g, int node, int visited[]) {
    if (node < 0 || node >= g->num_nodes || visited[node]) {
        return;
    }
    visited[node] = 1;
    printf("%s ", g->node_names[node]);
    for (int i = 0; i < g->num_nodes; i++) {
        if (g->adjacency[node][i] != 0 && !visited[i]) {
            depth_first_search(g, i, visited);
        }
    }
}

void breadth_first_search(struct Graph *g, int start_node) {
    if (start_node < 0 || start_node >= g->num_nodes) {
        return;
    }
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    visited[start_node] = 1;
    queue[rear++] = start_node;
    while (front < rear) {
        int current = queue[front++];
        printf("%s ", g->node_names[current]);
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[current][i] != 0 && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
}

void find_shortest_path(struct Graph *g, const char *start_name, const char *end_name) {
    int start = find_node_index(g, start_name);
    int end = find_node_index(g, end_name);
    if (start == -1 || end == -1) {
        printf("Invalid node names\n");
        return;
    }
    int dist[MAX_NODES];
    int prev[MAX_NODES];
    int visited[MAX_NODES] = {0};
    for (int i = 0; i < g->num_nodes; i++) {
        dist[i] = -1;
        prev[i] = -1;
    }
    dist[start] = 0;
    for (int count = 0; count < g->num_nodes - 1; count++) {
        int min_dist = -1;
        int min_node = -1;
        for (int i = 0; i < g->num_nodes; i++) {
            if (!visited[i] && dist[i] != -1 && (min_dist == -1 || dist[i] < min_dist)) {
                min_dist = dist[i];
                min_node = i;
            }
        }
        if (min_node == -1) break;
        visited[min_node] = 1;
        for (int i = 0; i < g->num_nodes; i++) {
            if (g->adjacency[min_node][i] != 0 && !visited[i]) {
                int new_dist = dist[min_node] + g->adjacency[min_node][i];
                if (dist[i] == -1 || new_dist < dist[i]) {
                    dist[i] = new_dist;
                    prev[i] = min_node;
                }
            }
        }
    }
    if (dist[end] == -1) {
        printf("No path exists\n");
        return;
    }
    printf("Shortest path distance: %d\nPath: ", dist[end]);
    int path[MAX_NODES];
    int path_len = 0;
    for (int at = end; at != -1; at = prev[at]) {
        path[path_len++] = at;
    }