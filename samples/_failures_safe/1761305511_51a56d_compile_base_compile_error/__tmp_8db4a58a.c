//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    int debug_count;
    char most_frequent_level[MAX_LEVEL_LENGTH];
    int max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_level_count = 0;
    memset(analyzer->stats.most_frequent_level, 0, MAX_LEVEL_LENGTH);
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) == 0) return 0;
    
    char timestamp[32] = {0};
    char level[MAX_LEVEL_LENGTH] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) == 0 || strlen(level) == 0 || strlen(message) == 0) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->severity = parse_severity(level);
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level) {
    analyzer->stats.total_entries++;
    
    if (strcmp(level, "ERROR") == 0) analyzer->stats.error_count++;
    else if (strcmp(level, "WARNING") == 0) analyzer->stats.warning_count++;
    else if (strcmp(level, "INFO") == 0) analyzer->stats.info_count++;
    else if (strcmp(level, "DEBUG") == 0) analyzer->stats.debug_count++;
}

void calculate_frequency_stats(struct LogAnalyzer *analyzer) {
    int error_count = 0, warning_count = 0, info_count = 0, debug_count = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        const char *level = analyzer->entries[i].level;
        if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
    }
    
    analyzer->stats.max_level_count = error_count;
    strncpy(analyzer->stats.most_frequent_level, "ERROR", MAX_LEVEL_LENGTH - 1);
    
    if (warning_count > analyzer->stats.max_level_count) {
        analyzer->stats.max_level_count = warning_count;
        strncpy(analyzer->stats.most_frequent_level, "WARNING", MAX_LEVEL_LENGTH - 1);
    }
    if (info_count > analyzer->stats.max_level_count) {
        analyzer->stats.max_level_count = info_count;
        strncpy(analyzer->stats.most_frequent_level, "INFO", MAX_LEVEL_LENGTH - 1);
    }
    if (debug_count > analyzer->stats.max_level_count) {
        analyzer->stats.max_level_count = debug_count;
        strncpy(analyzer->stats.most_frequent_level, "DEBUG", MAX_LEVEL_LENGTH - 1);
    }
}

void display_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Error count: %d\n", analyzer->stats.error_count);
    printf("Warning count: %d\n", analyzer->stats.warning_count);
    printf("Info count: %d\n", analyzer->stats.info_count);
    printf("Debug count: %d\n", analyzer->stats.debug_count);
    printf("Most frequent level: %s (%d occurrences)\n", 
           analyzer->stats.most_frequent_level, analyzer->stats.max_level_count);
    
    if (analy