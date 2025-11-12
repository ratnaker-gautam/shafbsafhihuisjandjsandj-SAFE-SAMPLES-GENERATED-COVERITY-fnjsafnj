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
    struct LogEntry most_frequent_ip;
    size_t ip_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    size_t entry_count;
    struct LogStats stats;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_requests = 0;
    analyzer->stats.total_bytes = 0;
    analyzer->stats.success_count = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.get_count = 0;
    analyzer->stats.post_count = 0;
    analyzer->stats.ip_count = 0;
    memset(&analyzer->stats.most_frequent_ip, 0, sizeof(struct LogEntry));
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    char ip[MAX_IP_LENGTH];
    char timestamp[32];
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char status[MAX_STATUS_LENGTH];
    unsigned long response_size;
    
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return 0;
    }
    
    int parsed = sscanf(line, "%15s - - [%31[^]]] \"%7s %255s\" %3s %lu",
                       ip, timestamp, method, path, status, &response_size);
    
    if (parsed != 6) {
        return 0;
    }
    
    if (strlen(ip) >= MAX_IP_LENGTH || strlen(timestamp) >= 32 ||
        strlen(method) >= MAX_METHOD_LENGTH || strlen(path) >= MAX_PATH_LENGTH ||
        strlen(status) >= MAX_STATUS_LENGTH) {
        return 0;
    }
    
    strncpy(entry->ip, ip, MAX_IP_LENGTH - 1);
    entry->ip[MAX_IP_LENGTH - 1] = '\0';
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->method, method, MAX_METHOD_LENGTH - 1);
    entry->method[MAX_METHOD_LENGTH - 1] = '\0';
    strncpy(entry->path, path, MAX_PATH_LENGTH - 1);
    entry->path[MAX_PATH_LENGTH - 1] = '\0';
    strncpy(entry->status, status, MAX_STATUS_LENGTH - 1);
    entry->status[MAX_STATUS_LENGTH - 1] = '\0';
    entry->response_size = response_size;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_requests++;
    analyzer->stats.total_bytes += entry->response_size;
    
    int status_code = atoi(entry->status);
    if (status_code >= 200 && status_code < 300) {
        analyzer->stats.success_count++;
    } else if (status_code >= 400) {
        analyzer->stats.error_count++;
    }
    
    if (strcmp(entry->method, "GET") == 0) {
        analyzer->stats.get_count++;
    } else if (strcmp(entry->method, "POST") == 0) {
        analyzer->stats.post_count++;
    }
    
    size_t current_ip_count = 0;
    for (size_t i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].ip, entry->ip) == 0) {
            current_ip_count++;
        }
    }
    
    if (current_ip_count > analyzer->stats.ip_count) {
        analyzer->stats.ip_count = current_ip_count;
        memcpy(&analyzer->stats.most_frequent_ip, entry, sizeof(struct LogEntry));
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) {
        return 0;
    }
    
    memcpy(&analyzer->entries[analyzer->entry_count], &entry, sizeof(struct LogEntry));
    analyzer->entry_count++;
    update_stats(analyzer, &entry);
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    const char *sample_logs[] = {
        "192.168.1.1 - - [10/Oct/2023:10:15:30 +0000] \"GET /index.html\" 200 1024",
        "192.168.1.2 - - [10/Oct