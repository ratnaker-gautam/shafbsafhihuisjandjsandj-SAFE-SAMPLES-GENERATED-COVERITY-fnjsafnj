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

struct LogTimestamp {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
};

struct LogEntry {
    char ip[MAX_IP_LENGTH];
    struct LogTimestamp timestamp;
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char status[MAX_STATUS_LENGTH];
    size_t response_size;
};

struct LogAnalysis {
    size_t total_requests;
    size_t total_bytes;
    size_t error_count;
    size_t success_count;
    struct LogEntry most_recent;
    char most_frequent_ip[MAX_IP_LENGTH];
    size_t max_ip_count;
};

struct LogParser {
    struct LogEntry entries[MAX_ENTRIES];
    size_t count;
    struct LogAnalysis analysis;
};

void initialize_parser(struct LogParser *parser) {
    parser->count = 0;
    parser->analysis.total_requests = 0;
    parser->analysis.total_bytes = 0;
    parser->analysis.error_count = 0;
    parser->analysis.success_count = 0;
    parser->analysis.max_ip_count = 0;
    memset(parser->analysis.most_frequent_ip, 0, MAX_IP_LENGTH);
    memset(&parser->analysis.most_recent, 0, sizeof(struct LogEntry));
}

int parse_timestamp(const char *str, struct LogTimestamp *ts) {
    if (str == NULL || ts == NULL) return 0;
    
    int day, month, year, hour, minute, second;
    if (sscanf(str, "%d/%d/%d:%d:%d:%d", &day, &month, &year, &hour, &minute, &second) != 6) {
        return 0;
    }
    
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) return 0;
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) return 0;
    
    ts->day = day;
    ts->month = month;
    ts->year = year;
    ts->hour = hour;
    ts->minute = minute;
    ts->second = second;
    return 1;
}

int validate_ip(const char *ip) {
    if (ip == NULL) return 0;
    size_t len = strlen(ip);
    if (len < 7 || len >= MAX_IP_LENGTH) return 0;
    
    int octets = 0;
    int digits = 0;
    for (size_t i = 0; i < len; i++) {
        if (ip[i] == '.') {
            if (digits == 0) return 0;
            octets++;
            digits = 0;
        } else if (isdigit(ip[i])) {
            digits++;
            if (digits > 3) return 0;
        } else {
            return 0;
        }
    }
    return (octets == 3 && digits > 0);
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
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
    
    if (!validate_ip(ip)) return 0;
    if (strlen(method) >= MAX_METHOD_LENGTH) return 0;
    if (strlen(path) >= MAX_PATH_LENGTH) return 0;
    if (strlen(status) >= MAX_STATUS_LENGTH) return 0;
    
    struct LogTimestamp ts;
    if (!parse_timestamp(timestamp_str, &ts)) return 0;
    
    strncpy(entry->ip, ip, MAX_IP_LENGTH - 1);
    entry->ip[MAX_IP_LENGTH - 1] = '\0';
    entry->timestamp = ts;
    strncpy(entry->method, method, MAX_METHOD_LENGTH - 1);
    entry->method[MAX_METHOD_LENGTH - 1] = '\0';
    strncpy(entry->path, path, MAX_PATH_LENGTH - 1);
    entry->path[MAX_PATH_LENGTH - 1] = '\0';
    strncpy(entry->status, status, MAX_STATUS_LENGTH - 1);
    entry->status[MAX_STATUS_LENGTH - 1] = '\0';
    entry->response_size = response_size;
    
    return 1;
}

int compare_timestamps(const struct LogTimestamp *a, const struct LogTimestamp *b) {
    if (a->year != b->year) return a->year - b->year;
    if (a->month != b