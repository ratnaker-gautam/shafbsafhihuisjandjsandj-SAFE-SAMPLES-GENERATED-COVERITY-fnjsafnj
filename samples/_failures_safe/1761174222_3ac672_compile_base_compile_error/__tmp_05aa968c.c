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
    struct LogStats stats;
    int entry_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_level_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "NONE");
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int validate_timestamp(const char *timestamp) {
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

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    if (strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    
    int level_counts[4] = {0};
    const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        switch (entry->severity) {
            case 1: stats->debug_count++; level_counts[0]++; break;
            case 2: stats->info_count++; level_counts[1]++; break;
            case 3: stats->warning_count++; level_counts[2]++; break;
            case 4: stats->error_count++; level_counts[3]++; break;
        }
    }
    
    int max_count = 0;
    int max_index = -1;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    if (max_index >= 0) {
        stats->max_level_count = max_count;
        strcpy(stats->most_frequent_level, level_names[max_index]);
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    const struct LogStats *stats = &analyzer->stats;
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Errors: %d\n", stats->error_count);
    printf("Warnings: %d\n", stats->warning_count);
    printf("Info: %d\n", stats->info_count);
    printf("Debug: %d\n", stats->debug_count);
    printf("Most frequent level: %s (%d occurrences)\n", 
           stats->most_frequent_level, stats->max_level_count);
}

void print_high_severity_entries(const struct LogAnalyzer *analyzer, int min_severity) {
    printf("\nHigh severity entries (severity >= %d):\n", min_severity);
    int count =