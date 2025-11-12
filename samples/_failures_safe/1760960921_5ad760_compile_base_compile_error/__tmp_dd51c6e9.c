//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    int level_counts[5] = {0};
    int max_count = 0;
    int max_severity = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        switch (entry->severity) {
            case 1: analyzer->stats.debug_count++; level_counts[1]++; break;
            case 2: analyzer->stats.info_count++; level_counts[2]++; break;
            case 3: analyzer->stats.warning_count++; level_counts[3]++; break;
            case 4: analyzer->stats.error_count++; level_counts[4]++; break;
        }
        
        if (entry->severity > max_severity) {
            max_severity = entry->severity;
        }
    }
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.max_severity = max_severity;
    
    for (int i = 1; i <= 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            switch (i) {
                case 1: strcpy(analyzer->stats.most_frequent_level, "DEBUG"); break;
                case 2: strcpy(analyzer->stats.most_frequent_level, "INFO"); break;
                case 3: strcpy(analyzer->stats.most_frequent_level, "WARNING"); break;
                case 4: strcpy(analyzer->stats.most_frequent_level, "ERROR"); break;
            }
        }
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Debug entries: %d\n", analyzer->stats.debug_count);
    printf("Info entries: %d\n", analyzer->stats.info_count);
    printf("Warning entries: %d\n", analyzer->stats.warning_count);
    printf("Error entries: %d\n", analyzer->stats.error_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    printf("Highest severity: %d\n", analyzer->stats.max_severity);
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    int severity = parse_severity(level);
    if (severity == 0) return;
    
    printf("\nEntries with level %s:\n", level);
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (analyzer->entries[i].severity == severity) {
            printf("[%s] %s: %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    add_log_entry(&analyzer, "2024-01-15 10:30