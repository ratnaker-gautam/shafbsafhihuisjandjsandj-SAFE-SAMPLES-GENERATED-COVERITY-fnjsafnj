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

void analyze_logs(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;

    int level_counts[5] = {0};
    char *level_names[] = {"UNKNOWN", "DEBUG", "INFO", "WARN", "ERROR"};

    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        switch (entry->severity) {
            case 4: stats->error_count++; break;
            case 3: stats->warning_count++; break;
            case 2: stats->info_count++; break;
            case 1: stats->debug_count++; break;
        }

        if (entry->severity >= 1 && entry->severity <= 4) {
            level_counts[entry->severity]++;
        }

        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
    }

    int max_count = 0;
    int max_level = 0;
    for (int i = 1; i <= 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_level = i;
        }
    }

    if (max_level > 0) {
        strcpy(stats->most_frequent_level, level_names[max_level]);
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
    
    char *max_severity_str = "UNKNOWN";
    switch (stats->max_severity) {
        case 4: max_severity_str = "ERROR"; break;
        case 3: max_severity_str = "WARN"; break;
        case 2: max_severity_str = "INFO"; break;
        case 1: max_severity_str = "DEBUG"; break;
    }
    printf("Maximum severity: %s\n", max_severity_str);
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);

    char timestamp[32];
    char level[16];
    char message[256];
    int entry_count = 0;

    printf("Enter log entries (timestamp level message). Type 'END' to finish.\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");

    while (entry_count < MAX_ENTRIES) {
        printf("Entry %d: ", entry_count + 1);
        
        if (scanf("%