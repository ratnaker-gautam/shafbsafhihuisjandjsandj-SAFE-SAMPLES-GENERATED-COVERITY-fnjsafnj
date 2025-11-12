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
    size_t ip_frequency;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    size_t entry_count;
    struct LogStats stats;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
}

int parse_log_line(const char *line, struct LogEntry *entry) {
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
    
    if (strlen(ip) >= MAX_IP_LENGTH) return 0;
    if (strlen(timestamp) >= 32) return 0;
    if (strlen(method) >= MAX_METHOD_LENGTH) return 0;
    if (strlen(path) >= MAX_PATH_LENGTH) return 0;
    if (strlen(status) >= MAX_STATUS_LENGTH) return 0;
    
    strcpy(entry->ip, ip);
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->method, method);
    strcpy(entry->path, path);
    strcpy(entry->status, status);
    entry->response_size = response_size;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
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
    
    if (current_ip_count > analyzer->stats.ip_frequency) {
        analyzer->stats.ip_frequency = current_ip_count;
        analyzer->stats.most_frequent_ip = *entry;
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (!analyzer || !line || analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) return 0;
    
    analyzer->entries[analyzer->entry_count] = entry;
    analyzer->entry_count++;
    
    update_stats(analyzer, &entry);
    return 1;
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Results:\n");
    printf("Total requests: %zu\n", analyzer->stats.total_requests);
    printf("Total bytes transferred: %zu\n", analyzer->stats.total_bytes);
    printf("Successful requests (2xx): %zu\n", analyzer->stats.success_count);
    printf("Error requests (4xx/5xx): %zu\n", analyzer->stats.error_count);
    printf("GET requests: %zu\n", analyzer->stats.get_count);
    printf("POST requests: %zu\n", analyzer->stats.post_count);
    
    if (analyzer->stats.ip_frequency > 0) {
        printf("Most frequent IP: %s (%zu requests)\n", 
               analyzer->stats.most_frequent_ip.ip, 
               analyzer->stats.ip_frequency);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    const char *sample_logs[] = {
        "192.168.1.1 - - [10/Oct/2023:14:30:01 +0000] \"GET /index