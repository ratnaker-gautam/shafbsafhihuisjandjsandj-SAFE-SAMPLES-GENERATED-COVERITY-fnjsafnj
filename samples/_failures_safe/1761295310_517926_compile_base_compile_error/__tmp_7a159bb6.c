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

void initialize_analyzer(struct LogAnalyzer *analyzer) {
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

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) == 0) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_entries++;
    
    if (entry->severity == 1) analyzer->stats.debug_count++;
    else if (entry->severity == 2) analyzer->stats.info_count++;
    else if (entry->severity == 3) analyzer->stats.warning_count++;
    else if (entry->severity == 4) analyzer->stats.error_count++;
    
    if (entry->severity > analyzer->stats.max_severity) {
        analyzer->stats.max_severity = entry->severity;
    }
}

void calculate_most_frequent_level(struct LogAnalyzer *analyzer) {
    int max_count = analyzer->stats.debug_count;
    strcpy(analyzer->stats.most_frequent_level, "DEBUG");
    
    if (analyzer->stats.info_count > max_count) {
        max_count = analyzer->stats.info_count;
        strcpy(analyzer->stats.most_frequent_level, "INFO");
    }
    if (analyzer->stats.warning_count > max_count) {
        max_count = analyzer->stats.warning_count;
        strcpy(analyzer->stats.most_frequent_level, "WARNING");
    }
    if (analyzer->stats.error_count > max_count) {
        strcpy(analyzer->stats.most_frequent_level, "ERROR");
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (line == NULL || strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    struct LogEntry new_entry;
    if (!parse_log_line(line, &new_entry)) return 0;
    
    analyzer->entries[analyzer->entry_count] = new_entry;
    update_stats(analyzer, &new_entry);
    analyzer->entry_count++;
    
    return 1;
}

void display_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("DEBUG entries: %d\n", analyzer->stats.debug_count);
    printf("INFO entries: %d\n", analyzer->stats.info_count);
    printf("WARNING entries: %d\n", analyzer->stats.warning_count);
    printf("ERROR entries: %d\n", analyzer->stats.error_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    printf("Highest severity: %d\n", analyzer->stats.max_severity);
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("Entries with level '%s':\n", level);
    int severity = parse_severity(level);
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (analyzer->entries[i].severity == severity) {
            printf("%s %s %s\n", analyzer->entries[i].