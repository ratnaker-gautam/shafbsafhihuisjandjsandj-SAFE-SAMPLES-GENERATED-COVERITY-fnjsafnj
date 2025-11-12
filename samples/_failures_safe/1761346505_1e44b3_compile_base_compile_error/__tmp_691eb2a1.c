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
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
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
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        const char *level = analyzer->entries[i].level;
        if (strcmp(level, "ERROR") == 0) analyzer->stats.error_count++;
        else if (strcmp(level, "WARN") == 0) analyzer->stats.warning_count++;
        else if (strcmp(level, "INFO") == 0) analyzer->stats.info_count++;
        else if (strcmp(level, "DEBUG") == 0) analyzer->stats.debug_count++;
    }
    
    int max_count = analyzer->stats.error_count;
    strcpy(analyzer->stats.most_frequent_level, "ERROR");
    
    if (analyzer->stats.warning_count > max_count) {
        max_count = analyzer->stats.warning_count;
        strcpy(analyzer->stats.most_frequent_level, "WARN");
    }
    if (analyzer->stats.info_count > max_count) {
        max_count = analyzer->stats.info_count;
        strcpy(analyzer->stats.most_frequent_level, "INFO");
    }
    if (analyzer->stats.debug_count > max_count) {
        strcpy(analyzer->stats.most_frequent_level, "DEBUG");
    }
}

void display_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("ERROR: %d\n", analyzer->stats.error_count);
    printf("WARN:  %d\n", analyzer->stats.warning_count);
    printf("INFO:  %d\n", analyzer->stats.info_count);
    printf("DEBUG: %d\n", analyzer->stats.debug_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("Entries with level %s:\n", level);
    int found = 0;
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("[%s] %s: %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No entries found with level %s\n", level);
    }
}

int main() {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    add_log_entry(&analyzer, "2024-01-15 10:30:25", "INFO", "System started successfully");
    add_log_entry(&analyzer, "2024-01-15 10:31:10", "DEBUG", "Initializing modules");
    add_log_entry(&analyzer, "2024-01-15 10: