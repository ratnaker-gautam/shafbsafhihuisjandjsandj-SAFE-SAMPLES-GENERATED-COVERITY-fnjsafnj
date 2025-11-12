//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    analyzer->stats.max_severity = 0;
}

int parse_timestamp(const char *timestamp) {
    struct tm tm = {0};
    if (strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    return 1;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 20 || strlen(level) >= 10 || strlen(message) >= 256) return 0;
    if (!parse_timestamp(timestamp) || !validate_level(level)) return 0;

    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);

    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
        analyzer->stats.error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
        analyzer->stats.warning_count++;
    } else {
        entry->severity = 1;
        analyzer->stats.info_count++;
    }

    if (entry->severity > analyzer->stats.max_severity) {
        analyzer->stats.max_severity = entry->severity;
    }

    analyzer->entry_count++;
    analyzer->stats.total_entries++;
    return 1;
}

void compute_stats(struct LogAnalyzer *analyzer) {
    int error_count = analyzer->stats.error_count;
    int warning_count = analyzer->stats.warning_count;
    int info_count = analyzer->stats.info_count;

    if (error_count >= warning_count && error_count >= info_count) {
        strcpy(analyzer->stats.most_frequent_level, "ERROR");
    } else if (warning_count >= error_count && warning_count >= info_count) {
        strcpy(analyzer->stats.most_frequent_level, "WARNING");
    } else {
        strcpy(analyzer->stats.most_frequent_level, "INFO");
    }
}

void display_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Errors: %d\n", analyzer->stats.error_count);
    printf("Warnings: %d\n", analyzer->stats.warning_count);
    printf("Info: %d\n", analyzer->stats.info_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    printf("Maximum severity: %d\n", analyzer->stats.max_severity);
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("Entries with level %s:\n", level);
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s [%s] %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
        }
    }
}

int main() {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);

    printf("Log Analyzer - Enter log entries (timestamp level message)\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message text\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'done' to finish input\n");

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "done", 4) == 0) break;

        char timestamp[20], level[10], message[256];
        if (sscanf(line, "%19s %9s %255[^\n