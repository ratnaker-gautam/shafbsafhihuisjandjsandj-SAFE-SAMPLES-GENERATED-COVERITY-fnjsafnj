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
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
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
    int max_count = 0;
    int max_severity = 0;

    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        switch (entry->severity) {
            case 1: stats->debug_count++; level_counts[1]++; break;
            case 2: stats->info_count++; level_counts[2]++; break;
            case 3: stats->warning_count++; level_counts[3]++; break;
            case 4: stats->error_count++; level_counts[4]++; break;
        }

        if (entry->severity > max_severity) {
            max_severity = entry->severity;
        }
    }

    for (int i = 1; i <= 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            switch (i) {
                case 1: strcpy(stats->most_frequent_level, "DEBUG"); break;
                case 2: strcpy(stats->most_frequent_level, "INFO"); break;
                case 3: strcpy(stats->most_frequent_level, "WARNING"); break;
                case 4: strcpy(stats->most_frequent_level, "ERROR"); break;
            }
        }
    }

    stats->max_severity = max_severity;
}

void print_stats(const struct LogAnalyzer *analyzer) {
    const struct LogStats *stats = &analyzer->stats;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Debug entries: %d\n", stats->debug_count);
    printf("Info entries: %d\n", stats->info_count);
    printf("Warning entries: %d\n", stats->warning_count);
    printf("Error entries: %d\n", stats->error_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Highest severity: %d\n", stats->max_severity);
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);

    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Enter 'END' to finish input\n");

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entries reached.\n");
            break;
        }

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (strcmp(line