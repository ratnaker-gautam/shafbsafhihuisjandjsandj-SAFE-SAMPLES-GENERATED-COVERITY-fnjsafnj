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
    char most_frequent_level[MAX_LEVEL_LENGTH];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_level_count = 0;
    memset(analyzer->stats.most_frequent_level, 0, MAX_LEVEL_LENGTH);
}

int validate_timestamp(const char *timestamp) {
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32], level[MAX_LEVEL_LENGTH], message[256];
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (result != 3) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->line_number = line_num;
    
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
    }
}

void calculate_frequent_level(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    uint32_t error_count = 0, warning_count = 0, info_count = 0;
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        const char *level = analyzer->entries[i].level;
        if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "INFO") == 0) info_count++;
    }
    
    if (error_count >= warning_count && error_count >= info_count) {
        strncpy(analyzer->stats.most_frequent_level, "ERROR", MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
        analyzer->stats.max_level_count = error_count;
    } else if (warning_count >= error_count && warning_count >= info_count) {
        strncpy(analyzer->stats.most_frequent_level, "WARNING", MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
        analyzer->stats.max_level_count = warning_count;
    } else {
        strncpy(analyzer->stats