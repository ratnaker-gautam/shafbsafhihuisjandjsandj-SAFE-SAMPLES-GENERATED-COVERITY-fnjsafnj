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
    char most_frequent_level[16];
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

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
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
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    int level_counts[4] = {0};
    int max_severity = 0;

    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (entry->severity == 3) error_count++;
        else if (entry->severity == 2) warning_count++;
        else if (entry->severity == 1) info_count++;

        level_counts[entry->severity]++;
        if (entry->severity > max_severity) max_severity = entry->severity;
    }

    int max_count = 0;
    char *most_frequent = "UNKNOWN";
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            if (i == 3) most_frequent = "ERROR";
            else if (i == 2) most_frequent = "WARN";
            else if (i == 1) most_frequent = "INFO";
            else most_frequent = "UNKNOWN";
        }
    }

    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = error_count;
    analyzer->stats.warning_count = warning_count;
    analyzer->stats.info_count = info_count;
    strncpy(analyzer->stats.most_frequent_level, most_frequent, 15);
    analyzer->stats.most_frequent_level[15] = '\0';
    analyzer->stats.max_severity = max_severity;
}

void print_stats(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Errors: %d\n", stats->error_count);
    printf("Warnings: %d\n", stats->warning_count);
    printf("Info: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    add_log_entry(analyzer, "2024-01-15 10:30:00", "INFO", "System started successfully");
    add_log_entry(analyzer, "2024-01-15 10:31:15", "WARN", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 10:32:30", "INFO", "User login: john_doe");
    add_log_entry(analyzer, "2024-01-15 10:33:45", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 10:34:20", "INFO", "Backup process started");
}

int main()