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

int parse_timestamp(const char *timestamp, struct tm *tm) {
    return sscanf(timestamp, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_entry(const char *line, struct LogEntry *entry) {
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    
    char level[10];
    char timestamp[20];
    char message[256];
    
    if (sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    struct tm time_struct;
    if (!parse_timestamp(timestamp, &time_struct)) return 0;
    
    if (strcmp(level, "ERROR") != 0 && strcmp(level, "WARNING") != 0 && strcmp(level, "INFO") != 0) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
    } else {
        entry->severity = 1;
    }
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!validate_log_entry(line, &entry)) return 0;
    
    analyzer->entries[analyzer->entry_count] = entry;
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
        } else if (strcmp(analyzer->entries[i].level, "WARNING") == 0) {
            analyzer->stats.warning_count++;
        } else {
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
        strcpy(analyzer->stats.most_frequent_level, "WARNING");
    } else {
        strcpy(analyzer->stats.most_frequent_level, "INFO");
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("ERROR count: %d\n", analyzer->stats.error_count);
    printf("WARNING count: %d\n", analyzer->stats.warning_count);
    printf("INFO count: %d\n", analyzer->stats.info_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent