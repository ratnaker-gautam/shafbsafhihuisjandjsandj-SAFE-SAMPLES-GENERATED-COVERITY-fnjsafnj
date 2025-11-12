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
    char message[MAX_LINE_LENGTH - 30];
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char earliest_timestamp[20];
    char latest_timestamp[20];
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
    memset(analyzer->stats.earliest_timestamp, 0, 20);
    memset(analyzer->stats.latest_timestamp, 0, 20);
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
    
    if (strlen(timestamp) >= 19 && strlen(level) < 10 && strlen(message) < (MAX_LINE_LENGTH - 30)) {
        strncpy(entry->timestamp, timestamp, 19);
        entry->timestamp[19] = '\0';
        strncpy(entry->level, level, 9);
        entry->level[9] = '\0';
        strncpy(entry->message, message, MAX_LINE_LENGTH - 31);
        entry->message[MAX_LINE_LENGTH - 31] = '\0';
        return 1;
    }
    
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        analyzer->entries[analyzer->entry_count] = entry;
        analyzer->entry_count++;
        return 1;
    }
    
    return 0;
}

void update_stats(struct LogAnalyzer *analyzer) {
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    
    if (analyzer->entry_count == 0) return;
    
    strcpy(analyzer->stats.earliest_timestamp, analyzer->entries[0].timestamp);
    strcpy(analyzer->stats.latest_timestamp, analyzer->entries[0].timestamp);
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, "ERROR") == 0) {
            analyzer->stats.error_count++;
        } else if (strcmp(analyzer->entries[i].level, "WARNING") == 0) {
            analyzer->stats.warning_count++;
        } else if (strcmp(analyzer->entries[i].level, "INFO") == 0) {
            analyzer->stats.info_count++;
        }
        
        if (strcmp(analyzer->entries[i].timestamp, analyzer->stats.earliest_timestamp) < 0) {
            strcpy(analyzer->stats.earliest_timestamp, analyzer->entries[i].timestamp);
        }
        if (strcmp(analyzer->entries[i].timestamp, analyzer->stats.latest_timestamp) > 0) {
            strcpy(analyzer->stats.latest_timestamp, analyzer->entries[i].timestamp);
        }
    }
}

void print_stats(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Error count: %d\n", stats->error_count);
    printf("Warning count: %d\n", stats->warning_count);
    printf("Info count: %d\n", stats->info_count);
    printf("Time range: %s to %s\n", stats->earliest_timestamp, stats->latest_timestamp);
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("\nEntries with level '%s':\n", level);
    int found = 0;
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s %s %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No entries found with level '%s'\n", level);
    }