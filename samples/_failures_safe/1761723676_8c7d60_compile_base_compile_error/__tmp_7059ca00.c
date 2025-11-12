//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[256];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char most_frequent_level[10];
    int max_severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    analyzer->stats.max_severity = 0;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    strncpy(timestamp, str, 19);
    timestamp[19] = '\0';
    return 1;
}

int parse_log_level(const char *str, char *level, int *severity) {
    if (strncmp(str, "ERROR", 5) == 0) {
        strcpy(level, "ERROR");
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARN", 4) == 0) {
        strcpy(level, "WARN");
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strcpy(level, "INFO");
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp_start = strchr(temp_line, '[');
    char *timestamp_end = strchr(temp_line, ']');
    
    if (!timestamp_start || !timestamp_end || timestamp_end <= timestamp_start) return 0;
    
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= 19) return 0;
    
    char timestamp_buf[20];
    strncpy(timestamp_buf, timestamp_start + 1, timestamp_len);
    timestamp_buf[timestamp_len] = '\0';
    
    if (!parse_timestamp(timestamp_buf, entry->timestamp)) return 0;
    
    char *level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char *level_end = strchr(level_start, ' ');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= 9) return 0;
    
    char level_buf[10];
    strncpy(level_buf, level_start, level_len);
    level_buf[level_len] = '\0';
    
    if (!parse_log_level(level_buf, entry->level, &entry->severity)) return 0;
    
    char *message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= 255) message_len = 255;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    analyzer->entry_count++;
    return 1;
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_severity = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            analyzer->stats.error_count++;
        } else if (strcmp(entry->level, "WARN") ==