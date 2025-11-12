//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
c
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
    int status_code;
    size_t response_size;
};

struct LogStats {
    size_t total_requests;
    size_t total_bytes;
    int error_count;
    int success_count;
    char most_frequent_ip[MAX_IP_LENGTH];
    size_t max_ip_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    size_t entry_count;
    struct LogStats stats;
};

int parse_timestamp(const char* str, time_t* timestamp) {
    struct tm tm = {0};
    if (strptime(str, "%d/%b/%Y:%H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *timestamp = mktime(&tm);
    return 1;
}

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char ip[MAX_IP_LENGTH];
    char timestamp_str[64];
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char status_str[MAX_STATUS_LENGTH];
    char size_str[32];
    
    int parsed = sscanf(line, "%15s - - [%63[^]]] \"%7s %255s\" %3s %31s",
                       ip, timestamp_str, method, path, status_str, size_str);
    
    if (parsed != 6) return 0;
    
    if (strlen(ip) >= MAX_IP_LENGTH) return 0;
    if (strlen(method) >= MAX_METHOD_LENGTH) return 0;
    if (strlen(path) >= MAX_PATH_LENGTH) return 0;
    if (strlen(status_str) >= MAX_STATUS_LENGTH) return 0;
    
    strncpy(entry->ip, ip, MAX_IP_LENGTH - 1);
    entry->ip[MAX_IP_LENGTH - 1] = '\0';
    
    if (!parse_timestamp(timestamp_str, &entry->timestamp)) return 0;
    
    strncpy(entry->method, method, MAX_METHOD_LENGTH - 1);
    entry->method[MAX_METHOD_LENGTH - 1] = '\0';
    
    strncpy(entry->path, path, MAX_PATH_LENGTH - 1);
    entry->path[MAX_PATH_LENGTH - 1] = '\0';
    
    char* endptr;
    long status = strtol(status_str, &endptr, 10);
    if (endptr == status_str || *endptr != '\0') return 0;
    if (status < 100 || status > 599) return 0;
    entry->status_code = (int)status;
    
    long size = strtol(size_str, &endptr, 10);
    if (endptr == size_str || *endptr != '\0') return 0;
    if (size < 0) return 0;
    entry->response_size = (size_t)size;
    
    return 1;
}

void initialize_analyzer(struct LogAnalyzer* analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_requests = 0;
    analyzer->stats.total_bytes = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.success_count = 0;
    analyzer->stats.max_ip_count = 0;
    analyzer->stats.most_frequent_ip[0] = '\0';
}

void update_stats(struct LogAnalyzer* analyzer, const struct LogEntry* entry) {
    if (analyzer == NULL || entry == NULL) return;
    
    analyzer->stats.total_requests++;
    analyzer->stats.total_bytes += entry->response_size;
    
    if (entry->status_code >= 400) {
        analyzer->stats.error_count++;
    } else if (entry->status_code >= 200 && entry->status_code < 300) {
        analyzer->stats.success_count++;
    }
}

void calculate_ip_stats(struct LogAnalyzer* analyzer) {
    if (analyzer == NULL) return;
    
    size_t ip_counts[MAX_ENTRIES] = {0};
    
    for (size_t i = 0; i < analyzer->entry_count; i++) {
        size_t count = 1;
        for (size_t j = i + 1; j < analyzer->entry_count; j++) {
            if (strcmp(analyzer->entries[i].ip, analyzer->entries[j].ip) == 0) {
                count++;
            }
        }
        ip_counts[i] = count;
    }
    
    size_t max_index = 0;
    for (size_t i = 1; i < analyzer->entry_count; i++) {
        if (ip_counts[i] > ip_counts[max_index]) {
            max_index = i;
        }
    }
    
    if (analyzer->entry_count > 0) {
        analyzer->stats.max_ip_count = ip_counts[max_index];
        strncpy