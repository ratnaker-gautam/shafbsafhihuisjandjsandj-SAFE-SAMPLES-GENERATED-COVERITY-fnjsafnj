//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    uint32_t debug_count;
    char most_frequent_level[MAX_LEVEL_LENGTH];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strncpy(analyzer->stats.most_frequent_level, "NONE", MAX_LEVEL_LENGTH - 1);
    analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    
    const char *valid_levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[MAX_LEVEL_LENGTH] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (!validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
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
}

void calculate_frequent_level(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    uint32_t level_counts[4] = {0};
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(analyzer->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    uint32_t max_count = 0;
    int max_index = -1;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    if (max_index != -1 && max_count > 0) {
        strncpy(analyzer->stats.most_frequent_level, levels[max_index], MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
        analyzer->stats.max_level_count = max_count;
    }
}

int process_log_data(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return 0;
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 DEBUG Initializing modules",
        "2024-01-15T10:32:45 WARNING Low disk space detected",
        "2024-01-15T10:33:20 ERROR Database connection failed",
        "2024-01-15T10:34:10 INFO User login successful",
        "2024-01-15T10:35:30 DEBUG Processing request ID",