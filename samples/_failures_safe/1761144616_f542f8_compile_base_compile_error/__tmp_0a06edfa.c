//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

void init_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_requests = 0;
    analyzer->stats.total_bytes = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.success_count = 0;
    analyzer->stats.max_ip_count = 0;
    memset(analyzer->stats.most_frequent_ip, 0, MAX_IP_LENGTH);
}

int parse_ip(const char *str, char *ip) {
    if (!str || !ip) return 0;
    size_t len = strlen(str);
    if (len >= MAX_IP_LENGTH) return 0;
    
    int dot_count = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '.') dot_count++;
        else if (!isdigit(str[i])) return 0;
    }
    if (dot_count != 3) return 0;
    
    strncpy(ip, str, MAX_IP_LENGTH - 1);
    ip[MAX_IP_LENGTH - 1] = '\0';
    return 1;
}

int parse_status(const char *str, int *status) {
    if (!str || !status) return 0;
    size_t len = strlen(str);
    if (len != 3) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(str[i])) return 0;
    }
    
    *status = atoi(str);
    return (*status >= 100 && *status <= 599);
}

int parse_size(const char *str, size_t *size) {
    if (!str || !size) return 0;
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') return 0;
    if (val < 0) return 0;
    *size = (size_t)val;
    return 1;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char ip[MAX_IP_LENGTH];
    char timestamp[32];
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char status_str[MAX_STATUS_LENGTH];
    char size_str[16];
    
    int result = sscanf(line, "%15s - - [%31[^]]] \"%7s %255[^\"]\" %3s %15s",
                       ip, timestamp, method, path, status_str, size_str);
    
    if (result != 6) return 0;
    
    if (!parse_ip(ip, entry->ip)) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    
    strncpy(entry->method, method, MAX_METHOD_LENGTH - 1);
    entry->method[MAX_METHOD_LENGTH - 1] = '\0';
    
    strncpy(entry->path, path, MAX_PATH_LENGTH - 1);
    entry->path[MAX_PATH_LENGTH - 1] = '\0';
    
    if (!parse_status(status_str, &entry->status_code)) return 0;
    if (!parse_size(size_str, &entry->response_size)) return 0;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_requests++;
    analyzer->stats.total_bytes += entry->response_size;
    
    if (entry->status_code >= 400) {
        analyzer->stats.error_count++;
    } else if (entry->status_code >= 200 && entry->status_code < 300) {
        analyzer->stats.success_count++;
    }
    
    size_t ip_count = 0;
    for (size_t i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].ip, entry->ip) == 0) {
            ip_count++;
        }
    }
    
    if (ip_count > analyzer->stats.max_ip_count) {
        analyzer->stats.max_ip_count = ip_count;
        strncpy(analyzer->stats.most_frequent_ip, entry->ip, MAX_IP_LENGTH - 1);
        analyzer->stats.most_frequent_ip[MAX_IP_LENGTH - 1] = '\0