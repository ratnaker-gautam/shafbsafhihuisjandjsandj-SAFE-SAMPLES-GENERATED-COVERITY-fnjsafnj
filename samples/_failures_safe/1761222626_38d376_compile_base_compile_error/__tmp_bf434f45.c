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
    time_t timestamp;
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
    analyzer->stats.max_ip_count = 0;
    memset(analyzer->stats.most_frequent_ip, 0, MAX_IP_LENGTH);
}

int parse_timestamp(const char *str, time_t *timestamp) {
    struct tm tm = {0};
    if (strptime(str, "%d/%b/%Y:%H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *timestamp = mktime(&tm);
    return 1;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    char ip[MAX_IP_LENGTH];
    char timestamp_str[64];
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char status[MAX_STATUS_LENGTH];
    unsigned long response_size;
    
    if (sscanf(line, "%15s - - [%63[^]]] \"%7s %255s\" %3s %lu",
               ip, timestamp_str, method, path, status, &response_size) != 6) {
        return 0;
    }
    
    if (strlen(ip) >= MAX_IP_LENGTH || strlen(method) >= MAX_METHOD_LENGTH ||
        strlen(path) >= MAX_PATH_LENGTH || strlen(status) >= MAX_STATUS_LENGTH) {
        return 0;
    }
    
    time_t ts;
    if (!parse_timestamp(timestamp_str, &ts)) {
        return 0;
    }
    
    strcpy(entry->ip, ip);
    entry->timestamp = ts;
    strcpy(entry->method, method);
    strcpy(entry->path, path);
    strcpy(entry->status, status);
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
    
    size_t ip_count = 0;
    for (size_t i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].ip, entry->ip) == 0) {
            ip_count++;
        }
    }
    
    if (ip_count > analyzer->stats.max_ip_count) {
        analyzer->stats.max_ip_count = ip_count;
        strcpy(analyzer->stats.most_frequent_ip, entry->ip);
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
    
    analyzer->entries[analyzer->entry_count] = entry;
    analyzer->entry_count++;
    update_stats(analyzer, &entry);
    
    return 1;
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total requests: %zu\n", analyzer->stats.total_requests);
    printf("Total bytes transferred: %zu\n", analyzer->stats.total_bytes);
    printf("Successful requests (2xx): %zu\n", analyzer->stats.success_count);
    printf("Error requests (4xx/5xx): %zu\n", analyzer->stats.error_count);
    printf("GET requests: %zu\n", analyzer->stats.get_count);
    printf("POST requests: %zu\n", analyzer->stats.post_count);
    printf("Most frequent IP: %s (%zu requests)\n", 
           analyzer->