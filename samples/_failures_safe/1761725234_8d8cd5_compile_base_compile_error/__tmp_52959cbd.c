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
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strncpy(analyzer->stats.most_frequent_level, "INFO", 9);
    analyzer->stats.most_frequent_level[9] = '\0';
    analyzer->stats.max_severity = 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    size_t line_len = strlen(line);
    if (line_len >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= 255) return 0;
    
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

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    if (entry->severity > analyzer->stats.max_severity) {
        analyzer->stats.max_severity = entry->severity;
    }
    
    if (analyzer->stats.error_count > analyzer->stats.warning_count && 
        analyzer->stats.error_count > analyzer->stats.info_count) {
        strncpy(analyzer->stats.most_frequent_level, "ERROR", 9);
        analyzer->stats.most_frequent_level[9] = '\0';
    } else if (analyzer->stats.warning_count > analyzer->stats.error_count && 
               analyzer->stats.warning_count > analyzer->stats.info_count) {
        strncpy(analyzer->stats.most_frequent_level, "WARNING", 9);
        analyzer->stats.most_frequent_level[9] = '\0';
    } else {
        strncpy(analyzer->stats.most_frequent_level, "INFO", 9);
        analyzer->stats.most_frequent_level[9] = '\0';
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry new_entry;
    if (!parse_log_line(line, &new_entry)) return 0;
    
    analyzer->entries[analyzer->entry_count] = new_entry;
    update_stats(analyzer, &new_entry);
    analyzer->entry_count++;
    
    return 1;
}

void display_stats(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("ERROR count: %d\n", analyzer->stats.error_count);
    printf("WARNING count: %d\n", analyzer->stats.warning_count);
    printf("INFO count: %d\n", analyzer->stats.info_count