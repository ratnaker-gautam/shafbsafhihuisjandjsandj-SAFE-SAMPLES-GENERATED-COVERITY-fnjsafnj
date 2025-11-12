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

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    analyzer->stats.max_severity = 0;
}

int parse_severity(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    return entry->severity > 0;
}

void add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    if (analyzer->entry_count >= MAX_ENTRIES) return;
    
    analyzer->entries[analyzer->entry_count] = *entry;
    analyzer->entry_count++;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_severity = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        switch (entry->severity) {
            case 1: analyzer->stats.debug_count++; break;
            case 2: analyzer->stats.info_count++; break;
            case 3: analyzer->stats.warning_count++; break;
            case 4: analyzer->stats.error_count++; break;
        }
        
        if (entry->severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = entry->severity;
        }
    }
    
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

void print_stats(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("DEBUG entries: %d\n", analyzer->stats.debug_count);
    printf("INFO entries: %d\n", analyzer->stats.info_count);
    printf("WARNING entries: %d\n", analyzer->stats.warning_count);
    printf("ERROR entries: %d\n", analyzer->stats.error_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    
    const char *max_severity_str = "UNKNOWN";
    switch (analyzer->stats.max_severity) {
        case 1: max_severity_str = "DEBUG"; break;
        case 2: max_severity_str = "INFO";