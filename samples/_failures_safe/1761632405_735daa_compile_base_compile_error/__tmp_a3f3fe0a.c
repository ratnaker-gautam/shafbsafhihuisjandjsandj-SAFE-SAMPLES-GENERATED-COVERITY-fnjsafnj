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
    char max_severity_message[256];
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
    analyzer->stats.max_severity = -1;
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
    strcpy(analyzer->stats.max_severity_message, "NONE");
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
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
    if (analyzer->entry_count == 0) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_severity = -1;
    
    int level_counts[4] = {0};
    const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        switch (entry->severity) {
            case 0: analyzer->stats.debug_count++; level_counts[0]++; break;
            case 1: analyzer->stats.info_count++; level_counts[1]++; break;
            case 2: analyzer->stats.warning_count++; level_counts[2]++; break;
            case 3: analyzer->stats.error_count++; level_counts[3]++; break;
        }
        
        if (entry->severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = entry->severity;
            strncpy(analyzer->stats.max_severity_message, entry->message, 255);
            analyzer->stats.max_severity_message[255] = '\0';
        }
    }
    
    int max_count = 0;
    int max_index = 0;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    strncpy(analyzer->stats.most_frequent_level, level_names[max_index], 15);
    analyzer->stats.most_frequent_level[15] = '\0';
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("DEBUG: %d\n", analyzer->stats.debug_count);
    printf("INFO: %d\n", analyzer->stats.info_count);
    printf("WARNING: %d\n", analyzer->stats.warning_count);
    printf("ERROR: %d\n", analyzer->stats.error_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    
    if (analyzer->stats.max_severity >= 0) {
        const char *severity_name = "UNKNOWN";
        switch (analyzer->stats.max_severity) {
            case 0: severity_name = "DEBUG"; break;
            case 1: severity