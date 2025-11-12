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

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    analyzer->stats.max_severity = 0;
}

int parse_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 20 || strlen(level) >= 10 || strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->severity = parse_log_level(level);
    if (entry->severity == -1) return 0;
    
    analyzer->entry_count++;
    return 1;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_severity = 0;
    
    int level_counts[4] = {0};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (entry->severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = entry->severity;
        }
        
        switch (entry->severity) {
            case 3: analyzer->stats.error_count++; level_counts[3]++; break;
            case 2: analyzer->stats.warning_count++; level_counts[2]++; break;
            case 1: analyzer->stats.info_count++; level_counts[1]++; break;
            case 0: level_counts[0]++; break;
        }
    }
    
    int max_count = 0;
    int max_index = 1;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    switch (max_index) {
        case 3: strcpy(analyzer->stats.most_frequent_level, "ERROR"); break;
        case 2: strcpy(analyzer->stats.most_frequent_level, "WARN"); break;
        case 1: strcpy(analyzer->stats.most_frequent_level, "INFO"); break;
        case 0: strcpy(analyzer->stats.most_frequent_level, "DEBUG"); break;
    }
}

void print_analysis(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Errors: %d\n", analyzer->stats.error_count);
    printf("Warnings: %d\n", analyzer->stats.warning_count);
    printf("Info messages: %d\n", analyzer->stats.info_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    printf("Highest severity: %d\n", analyzer->stats.max_severity);
    printf("\nSample entries:\n");
    
    int samples = analyzer->entry_count < 3 ? analyzer->entry_count : 3;
    for (int i = 0; i < samples; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
    }
}

int main() {
    struct LogAnalyzer analyzer;
    char line[MAX_LINE_LENGTH];
    char timestamp[20];
    char level[10];
    char message[256];