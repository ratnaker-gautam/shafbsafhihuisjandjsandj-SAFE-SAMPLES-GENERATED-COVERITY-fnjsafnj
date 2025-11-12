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
    int min_severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
    analyzer->stats.max_severity = -1;
    analyzer->stats.min_severity = 1000;
}

int parse_severity(const char *level) {
    if (!level) return 0;
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (!analyzer || analyzer->entry_count >= MAX_ENTRIES) return 0;
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
    if (!analyzer) return;
    
    int level_counts[5] = {0};
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_severity = -1;
    analyzer->stats.min_severity = 1000;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        switch (entry->severity) {
            case 1: analyzer->stats.debug_count++; level_counts[1]++; break;
            case 2: analyzer->stats.info_count++; level_counts[2]++; break;
            case 3: analyzer->stats.warning_count++; level_counts[3]++; break;
            case 4: analyzer->stats.error_count++; level_counts[4]++; break;
        }
        
        if (entry->severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = entry->severity;
        }
        if (entry->severity < analyzer->stats.min_severity && entry->severity > 0) {
            analyzer->stats.min_severity = entry->severity;
        }
    }
    
    int max_count = 0;
    int max_index = 0;
    for (int i = 1; i <= 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    switch (max_index) {
        case 1: strcpy(analyzer->stats.most_frequent_level, "DEBUG"); break;
        case 2: strcpy(analyzer->stats.most_frequent_level, "INFO"); break;
        case 3: strcpy(analyzer->stats.most_frequent_level, "WARNING"); break;
        case 4: strcpy(analyzer->stats.most_frequent_level, "ERROR"); break;
        default: strcpy(analyzer->stats.most_frequent_level, "UNKNOWN"); break;
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Debug entries: %d\n", analyzer->stats.debug_count);
    printf("Info entries: %d\n", analyzer->stats.info_count);
    printf("Warning entries: %d\n", analyzer->stats.warning_count);
    printf