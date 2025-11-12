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
    char most_frequent_level[16];
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
    strncpy(analyzer->stats.most_frequent_level, "UNKNOWN", sizeof(analyzer->stats.most_frequent_level) - 1);
    analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, sizeof(temp_line) - 1);
    temp_line[sizeof(temp_line) - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(level, "WARNING") == 0) entry->severity = 2;
    else if (strcmp(level, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (analyzer->entries[i].severity == 3) analyzer->stats.error_count++;
        else if (analyzer->entries[i].severity == 2) analyzer->stats.warning_count++;
        else if (analyzer->entries[i].severity == 1) analyzer->stats.info_count++;
    }
    
    if (analyzer->stats.error_count >= analyzer->stats.warning_count && 
        analyzer->stats.error_count >= analyzer->stats.info_count) {
        strncpy(analyzer->stats.most_frequent_level, "ERROR", sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
    } else if (analyzer->stats.warning_count >= analyzer->stats.error_count && 
               analyzer->stats.warning_count >= analyzer->stats.info_count) {
        strncpy(analyzer->stats.most_frequent_level, "WARNING", sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
    } else {
        strncpy(analyzer->stats.most_frequent_level, "INFO", sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry new_entry;
    if (parse_log_line(line,