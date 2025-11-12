//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_IP_LENGTH 16
#define MAX_METHOD_LENGTH 8
#define MAX_PATH_LENGTH 256
#define MAX_STATUS_LENGTH 4

struct LogEntry {
    char ip[MAX_IP_LENGTH];
    char timestamp[32];
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    int status_code;
    size_t response_size;
};

struct LogStats {
    size_t total_requests;
    size_t total_bytes;
    int error_count;
    int success_count;
    struct {
        char ip[MAX_IP_LENGTH];
        size_t count;
    } top_client;
};

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char ip[MAX_IP_LENGTH] = {0};
    char timestamp[32] = {0};
    char method[MAX_METHOD_LENGTH] = {0};
    char path[MAX_PATH_LENGTH] = {0};
    char status[MAX_STATUS_LENGTH] = {0};
    char size_str[16] = {0};
    
    int parsed = sscanf(line, "%15s - - [%31[^]]] \"%7s %255s %*[^\"]\" %3s %15s",
                       ip, timestamp, method, path, status, size_str);
    
    if (parsed < 6) return 0;
    
    if (strlen(ip) >= MAX_IP_LENGTH - 1) return 0;
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(method) >= MAX_METHOD_LENGTH - 1) return 0;
    if (strlen(path) >= MAX_PATH_LENGTH - 1) return 0;
    if (strlen(status) >= MAX_STATUS_LENGTH - 1) return 0;
    
    strncpy(entry->ip, ip, MAX_IP_LENGTH - 1);
    strncpy(entry->timestamp, timestamp, 31);
    strncpy(entry->method, method, MAX_METHOD_LENGTH - 1);
    strncpy(entry->path, path, MAX_PATH_LENGTH - 1);
    
    entry->status_code = atoi(status);
    if (entry->status_code < 100 || entry->status_code > 599) return 0;
    
    entry->response_size = (size_t)atol(size_str);
    if (entry->response_size > SIZE_MAX / 2) return 0;
    
    return 1;
}

void initialize_stats(struct LogStats *stats) {
    if (stats == NULL) return;
    stats->total_requests = 0;
    stats->total_bytes = 0;
    stats->error_count = 0;
    stats->success_count = 0;
    memset(stats->top_client.ip, 0, MAX_IP_LENGTH);
    stats->top_client.count = 0;
}

void update_stats(const struct LogEntry *entry, struct LogStats *stats) {
    if (entry == NULL || stats == NULL) return;
    
    stats->total_requests++;
    stats->total_bytes += entry->response_size;
    
    if (entry->status_code >= 200 && entry->status_code < 300) {
        stats->success_count++;
    } else if (entry->status_code >= 400) {
        stats->error_count++;
    }
}

void update_top_client(const char *ip, struct LogStats *stats) {
    if (ip == NULL || stats == NULL) return;
    
    static size_t ip_counts[MAX_ENTRIES] = {0};
    static char ips[MAX_ENTRIES][MAX_IP_LENGTH] = {0};
    static size_t ip_count = 0;
    
    for (size_t i = 0; i < ip_count; i++) {
        if (strcmp(ips[i], ip) == 0) {
            ip_counts[i]++;
            if (ip_counts[i] > stats->top_client.count) {
                stats->top_client.count = ip_counts[i];
                strncpy(stats->top_client.ip, ip, MAX_IP_LENGTH - 1);
            }
            return;
        }
    }
    
    if (ip_count < MAX_ENTRIES) {
        strncpy(ips[ip_count], ip, MAX_IP_LENGTH - 1);
        ip_counts[ip_count] = 1;
        if (stats->top_client.count == 0) {
            stats->top_client.count = 1;
            strncpy(stats->top_client.ip, ip, MAX_IP_LENGTH - 1);
        }
        ip_count++;
    }
}

void print_stats(const struct LogStats *stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total Requests: %zu\n", stats->total_requests);
    printf("Total Bytes Transferred: %zu\n", stats->total_bytes);
    printf("Successful Requests (2xx): %d\n", stats->success_count);
    printf("Error Requests (4xx/5xx): %d\n", stats->error_count);
    
    if (stats->top_client.count > 0) {
        printf("Most Frequent Client: %s (%zu requests)\n", 
               stats->top_client.ip