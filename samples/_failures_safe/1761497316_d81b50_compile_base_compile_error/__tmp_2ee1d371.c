//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    struct LogStats stats;
    int entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
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
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = parse_severity(level);

    analyzer->entry_count++;
    return 1;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    stats->max_severity = 0;

    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
        if (strcmp(entry->level, "ERROR") == 0) stats->error_count++;
        else if (strcmp(entry->level, "WARN") == 0) stats->warning_count++;
        else if (strcmp(entry->level, "INFO") == 0) stats->info_count++;
        else if (strcmp(entry->level, "DEBUG") == 0) stats->debug_count++;
    }

    int max_count = stats->error_count;
    strcpy(stats->most_frequent_level, "ERROR");
    if (stats->warning_count > max_count) {
        max_count = stats->warning_count;
        strcpy(stats->most_frequent_level, "WARN");
    }
    if (stats->info_count > max_count) {
        max_count = stats->info_count;
        strcpy(stats->most_frequent_level, "INFO");
    }
    if (stats->debug_count > max_count) {
        strcpy(stats->most_frequent_level, "DEBUG");
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
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

void print_sample_entries(const struct LogAnalyzer *analyzer, int count) {
    if (count <= 0 || count > analyzer->entry_count) count = analyzer->entry_count;
    printf("\nSample entries (first %d):\n", count);
    for (int i = 0; i < count && i < analyzer->entry_count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
    }
}

int main