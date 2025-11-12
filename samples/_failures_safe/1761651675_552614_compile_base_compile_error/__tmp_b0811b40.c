//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

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
    char status[MAX_STATUS_LENGTH];
    size_t response_size;
};

struct LogStats {
    size_t total_requests;
    size_t total_bytes;
    size_t success_count;
    size_t error_count;
    size_t get_count;
    size_t post_count;
    char most_frequent_ip[MAX_IP_LENGTH];
    size_t max_ip_count;
};

struct IPCounter {
    char ip[MAX_IP_LENGTH];
    size_t count;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char ip[MAX_IP_LENGTH] = {0};
    char timestamp[32] = {0};
    char method[MAX_METHOD_LENGTH] = {0};
    char path[MAX_PATH_LENGTH] = {0};
    char status[MAX_STATUS_LENGTH] = {0};
    unsigned long response_size = 0;
    
    int parsed = sscanf(line, "%15s - - [%31[^]]] \"%7s %255s\" %3s %lu",
                       ip, timestamp, method, path, status, &response_size);
    
    if (parsed != 6) return 0;
    
    if (strlen(ip) >= MAX_IP_LENGTH - 1) return 0;
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(method) >= MAX_METHOD_LENGTH - 1) return 0;
    if (strlen(path) >= MAX_PATH_LENGTH - 1) return 0;
    if (strlen(status) >= MAX_STATUS_LENGTH - 1) return 0;
    
    strcpy(entry->ip, ip);
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->method, method);
    strcpy(entry->path, path);
    strcpy(entry->status, status);
    entry->response_size = response_size;
    
    return 1;
}

void initialize_stats(struct LogStats* stats) {
    if (!stats) return;
    memset(stats, 0, sizeof(struct LogStats));
    stats->most_frequent_ip[0] = '\0';
}

void update_stats(struct LogStats* stats, const struct LogEntry* entry, struct IPCounter* ip_counters, size_t* unique_ips) {
    if (!stats || !entry) return;
    
    stats->total_requests++;
    stats->total_bytes += entry->response_size;
    
    int status_code = atoi(entry->status);
    if (status_code >= 200 && status_code < 300) {
        stats->success_count++;
    } else if (status_code >= 400) {
        stats->error_count++;
    }
    
    if (strcmp(entry->method, "GET") == 0) {
        stats->get_count++;
    } else if (strcmp(entry->method, "POST") == 0) {
        stats->post_count++;
    }
    
    int found = 0;
    for (size_t i = 0; i < *unique_ips; i++) {
        if (strcmp(ip_counters[i].ip, entry->ip) == 0) {
            ip_counters[i].count++;
            if (ip_counters[i].count > stats->max_ip_count) {
                stats->max_ip_count = ip_counters[i].count;
                strcpy(stats->most_frequent_ip, entry->ip);
            }
            found = 1;
            break;
        }
    }
    
    if (!found && *unique_ips < MAX_ENTRIES) {
        strcpy(ip_counters[*unique_ips].ip, entry->ip);
        ip_counters[*unique_ips].count = 1;
        if (stats->max_ip_count == 0) {
            stats->max_ip_count = 1;
            strcpy(stats->most_frequent_ip, entry->ip);
        }
        (*unique_ips)++;
    }
}

void print_stats(const struct LogStats* stats) {
    if (!stats) return;
    
    printf("Log Analysis Results:\n");
    printf("Total Requests: %zu\n", stats->total_requests);
    printf("Total Bytes Transferred: %zu\n", stats->total_bytes);
    printf("Successful Requests (2xx): %zu\n", stats->success_count);
    printf("Error Requests (4xx/5xx): %zu\n", stats->error_count);
    printf("GET Requests: %zu\n", stats->get_count);
    printf("POST Requests: %zu\n", stats->post_count);
    
    if (stats->most_frequent_ip[0] != '\0') {
        printf("Most Frequent IP: %s (%zu requests)\n", stats->most_frequent_ip, stats->max_ip_count);
    }
    
    if (stats->total_requests > 0) {
        double success_rate = (double)stats->success_count / stats->total_requests * 100.0;
        double avg_bytes =