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

void initialize_stats(struct LogStats *stats) {
    if (stats == NULL) return;
    stats->total_entries = 0;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    strncpy(stats->most_frequent_level, "INFO", sizeof(stats->most_frequent_level) - 1);
    stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = '\0';
    stats->max_severity = 0;
}

int parse_severity(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_entry(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) == 0 || strlen(level) == 0 || strlen(message) == 0) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = parse_severity(level);
    
    return 1;
}

void update_stats(struct LogStats *stats, const struct LogEntry *entry) {
    if (stats == NULL || entry == NULL) return;
    
    stats->total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        stats->error_count++;
    } else if (strcmp(entry->level, "WARN") == 0) {
        stats->warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        stats->info_count++;
    }
    
    if (entry->severity > stats->max_severity) {
        stats->max_severity = entry->severity;
    }
}

void determine_most_frequent(struct LogStats *stats) {
    if (stats == NULL) return;
    
    if (stats->error_count >= stats->warning_count && stats->error_count >= stats->info_count) {
        strncpy(stats->most_frequent_level, "ERROR", sizeof(stats->most_frequent_level) - 1);
        stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = '\0';
    } else if (stats->warning_count >= stats->error_count && stats->warning_count >= stats->info_count) {
        strncpy(stats->most_frequent_level, "WARN", sizeof(stats->most_frequent_level) - 1);
        stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = '\0';
    } else {
        strncpy(stats->most_frequent_level, "INFO", sizeof(stats->most_frequent_level) - 1);
        stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = '\0';
    }
}

void print_stats(const struct LogStats *stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("WARN entries: %d\n", stats->warning_count);
    printf("INFO entries: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    initialize_stats(&stats);
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started