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

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
    analyzer->stats.max_severity = 0;
}

int parse_timestamp(const char *timestamp) {
    struct tm tm = {0};
    if (strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    return 1;
}

int validate_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int calculate_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    if (strlen(timestamp) >= 20 || strlen(level) >= 10 || strlen(message) >= 256) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp) || !validate_log_level(level)) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = calculate_severity(level);
    
    analyzer->entry_count++;
    return 1;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    int level_counts[3] = {0};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
            level_counts[0]++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
            level_counts[1]++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
            level_counts[2]++;
        }
        
        if (entry->severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = entry->severity;
        }
    }
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = error_count;
    analyzer->stats.warning_count = warning_count;
    analyzer->stats.info_count = info_count;
    
    int max_count = 0;
    int max_index = -1;
    for (int i = 0; i < 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    if (max_index == 0) strcpy(analyzer->stats.most_frequent_level, "ERROR");
    else if (max_index == 1) strcpy(analyzer->stats.most_frequent_level, "WARNING");
    else if (max_index == 2) strcpy(analyzer->stats.most_frequent_level, "INFO");
}

void print_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Errors: %d\n", analyzer->stats.error_count);
    printf("Warnings: %d\n", analyzer->stats.warning_count);
    printf("Info: %d\n", analyzer->stats.info_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    printf("Maximum severity: %d\n", analyzer->stats.max_severity);
    printf("\n");
}

void print_high_severity_logs(const struct LogAnalyzer *analyzer, int min_severity) {
    printf("High severity logs (severity