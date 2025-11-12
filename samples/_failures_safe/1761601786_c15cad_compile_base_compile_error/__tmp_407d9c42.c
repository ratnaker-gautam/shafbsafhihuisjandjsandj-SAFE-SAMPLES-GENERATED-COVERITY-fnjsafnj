//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    strncpy(timestamp, str, 19);
    timestamp[19] = '\0';
    return 1;
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    char timestamp[20], level[10], message[256];
    int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    if (!parse_timestamp(timestamp, analyzer->entries[analyzer->entry_count].timestamp)) return 0;
    
    strncpy(analyzer->entries[analyzer->entry_count].level, level, 9);
    analyzer->entries[analyzer->entry_count].level[9] = '\0';
    strncpy(analyzer->entries[analyzer->entry_count].message, message, 255);
    analyzer->entries[analyzer->entry_count].message[255] = '\0';
    
    analyzer->entries[analyzer->entry_count].severity = parse_severity(level);
    analyzer->entry_count++;
    return 1;
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_severity = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, "ERROR") == 0) {
            analyzer->stats.error_count++;
        } else if (strcmp(analyzer->entries[i].level, "WARN") == 0) {
            analyzer->stats.warning_count++;
        } else if (strcmp(analyzer->entries[i].level, "INFO") == 0) {
            analyzer->stats.info_count++;
        }
        
        if (analyzer->entries[i].severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = analyzer->entries[i].severity;
        }
    }
    
    if (analyzer->stats.error_count >= analyzer->stats.warning_count && 
        analyzer->stats.error_count >= analyzer->stats.info_count) {
        strcpy(analyzer->stats.most_frequent_level, "ERROR");
    } else if (analyzer->stats.warning_count >= analyzer->stats.info_count) {
        strcpy(analyzer->stats.most_frequent_level, "WARN");
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
    printf("\nEntries with level %s:\n", level);
    int found =