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
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    analyzer->stats.max_severity = 0;
}

int parse_timestamp(const char *timestamp) {
    if (timestamp == NULL) return 0;
    struct tm tm = {0};
    if (strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    return 1;
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 20) return 0;
    if (strlen(level) >= 10) return 0;
    if (strlen(message) >= 256) return 0;
    if (!parse_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;

    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';

    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
    } else {
        entry->severity = 1;
    }

    analyzer->entry_count++;
    return 1;
}

void compute_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->max_severity = 0;

    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        if (strcmp(entry->level, "ERROR") == 0) {
            stats->error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            stats->warning_count++;
        } else {
            stats->info_count++;
        }
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
    }

    if (stats->error_count >= stats->warning_count && stats->error_count >= stats->info_count) {
        strcpy(stats->most_frequent_level, "ERROR");
    } else if (stats->warning_count >= stats->error_count && stats->warning_count >= stats->info_count) {
        strcpy(stats->most_frequent_level, "WARNING");
    } else {
        strcpy(stats->most_frequent_level, "INFO");
    }
}

void display_stats(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    const struct LogStats *stats = &analyzer->stats;
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Errors: %d\n", stats->error_count);
    printf("Warnings: %d\n", stats->warning_count);
    printf("Info: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer