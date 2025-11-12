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

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) < 30) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    if (strlen(timestamp) >= 20 || strlen(level) >= 10 || strlen(message) >= 256) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
    } else if (strcmp(level, "INFO") == 0) {
        entry->severity = 1;
    } else {
        entry->severity = 0;
    }
    
    return 1;
}

void add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer->entry_count >= MAX_ENTRIES) return;
    
    analyzer->entries[analyzer->entry_count] = *entry;
    analyzer->entry_count++;
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_severity = 0;
    
    int level_counts[3] = {0};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        
        if (entry->severity == 3) {
            analyzer->stats.error_count++;
            level_counts[2]++;
        } else if (entry->severity == 2) {
            analyzer->stats.warning_count++;
            level_counts[1]++;
        } else if (entry->severity == 1) {
            analyzer->stats.info_count++;
            level_counts[0]++;
        }
        
        if (entry->severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = entry->severity;
        }
    }
    
    int max_count = level_counts[0];
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    
    if (level_counts[1] > max_count) {
        max_count = level_counts[1];
        strcpy(analyzer->stats.most_frequent_level, "WARNING");
    }
    if (level_counts[2] > max_count) {
        strcpy(analyzer->stats.most_frequent_level, "ERROR");
    }
}

void display_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("ERROR entries: %d\n", analyzer->stats.error_count);
    printf("WARNING entries: %d\n", analyzer->stats.warning_count);
    printf("INFO entries: %d\n", analyzer->stats.info_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    printf("Maximum severity: %d\n", analyzer->stats.max_severity);
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (level == NULL) return;
    printf("\nEntries with level %s:\n", level);
    int found = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        if (strcmp(entry