//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LEN 10
#define MAX_MESSAGE_LEN 512

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LEN];
    char message[MAX_MESSAGE_LEN];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    uint32_t debug_count;
    char most_frequent_level[MAX_LEVEL_LEN];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(*analyzer));
    strncpy(analyzer->stats.most_frequent_level, "NONE", MAX_LEVEL_LEN - 1);
    analyzer->stats.most_frequent_level[MAX_LEVEL_LEN - 1] = '\0';
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level_buf[MAX_LEVEL_LEN];
    char timestamp_buf[32];
    char message_buf[MAX_MESSAGE_LEN];
    
    int parsed = sscanf(line, "%31s %9s %511[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp_buf) >= sizeof(entry->timestamp) ||
        strlen(level_buf) >= sizeof(entry->level) ||
        strlen(message_buf) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp_buf, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    for (char *p = entry->level; *p; p++) {
        *p = (char)toupper((unsigned char)*p);
    }
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level) {
    if (!analyzer || !level) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(level, "INFO") == 0) {
        analyzer->stats.info_count++;
    } else if (strcmp(level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
    
    uint32_t current_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        current_count = analyzer->stats.error_count;
    } else if (strcmp(level, "WARNING") == 0) {
        current_count = analyzer->stats.warning_count;
    } else if (strcmp(level, "INFO") == 0) {
        current_count = analyzer->stats.info_count;
    } else if (strcmp(level, "DEBUG") == 0) {
        current_count = analyzer->stats.debug_count;
    }
    
    if (current_count > analyzer->stats.max_level_count) {
        analyzer->stats.max_level_count = current_count;
        strncpy(analyzer->stats.most_frequent_level, level, MAX_LEVEL_LEN - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LEN - 1] = '\0';
    }
}

int process_log_data(struct LogAnalyzer *analyzer) {
    if (!analyzer) return 0;
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 DEBUG Loading configuration file",
        "2024-01-15T10:32:30 WARNING Low disk space on /var",
        "2024-01-15T10:33:45 INFO User login: john_doe",
        "2024-01-15T10:35:00 ERROR Database connection failed",
        "2024-01-15T10:36:15 INFO Backup completed",
        "2024-01-15T10:37:30 DEBUG Processing request ID 12345",
        "2024-01-15T10:38:45 WARNING High memory usage detected",
        "2024-01-15T10:40:00 INFO Scheduled maintenance started",
        "2024-01-15T10:41:15 ERROR File not found: /tmp/data.xml