//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    int debug_count;
    char most_frequent_level[16];
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
    analyzer->stats.debug_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
    analyzer->stats.max_severity = 0;
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARN") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;

    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    analyzer->entry_count++;
    return 1;
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    stats->max_severity = 0;

    int level_counts[5] = {0};
    const char *level_names[] = {"UNKNOWN", "DEBUG", "INFO", "WARN", "ERROR"};

    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        if (entry->severity >= 1 && entry->severity <= 4) {
            level_counts[entry->severity]++;
        }
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
    }

    stats->debug_count = level_counts[1];
    stats->info_count = level_counts[2];
    stats->warning_count = level_counts[3];
    stats->error_count = level_counts[4];

    int max_count = 0;
    int max_index = 0;
    for (int i = 1; i <= 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    if (max_count > 0) {
        strcpy(stats->most_frequent_level, level_names[max_index]);
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    const struct LogStats *stats = &analyzer->stats;
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Error count: %d\n", stats->error_count);
    printf("Warning count: %d\n", stats->warning_count);
    printf("Info count: %d\n", stats->info_count);
    printf("Debug count: %d\n", stats->debug_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("Entries with level %s:\n", level);
    int severity = parse_severity(level);
    int count = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        if (entry->severity == severity) {
            printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
            count++;
        }
    }
    if (count == 0) {
        printf("No entries found for level %s\n", level);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);

    add_log_entry(&analy