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

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) == 0) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    return 1;
}

void add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer->entry_count >= MAX_ENTRIES) return;
    
    analyzer->entries[analyzer->entry_count] = *entry;
    analyzer->entry_count++;
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    stats->max_severity = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) stats->error_count++;
        else if (strcmp(entry->level, "WARN") == 0) stats->warning_count++;
        else if (strcmp(entry->level, "INFO") == 0) stats->info_count++;
        else if (strcmp(entry->level, "DEBUG") == 0) stats->debug_count++;
        
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
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

void print_stats(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR count: %d\n", stats->error_count);
    printf("WARN count: %d\n", stats->warning_count);
    printf("INFO count: %d\n", stats->info_count);
    printf("DEBUG count: %d\n", stats->debug_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

void print_entries_by_level(struct LogAnalyzer *analyzer, const char *level) {
    printf("\nEntries with level %s:\n", level);
    int found = 0;
    
    for (int i = 0;