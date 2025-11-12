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

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    analyzer->stats.max_severity = 0;
}

int parse_timestamp(const char *timestamp_str, struct tm *tm) {
    return sscanf(timestamp_str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_level(const char *level) {
    return (strcmp(level, "ERROR") == 0 ||
            strcmp(level, "WARNING") == 0 ||
            strcmp(level, "INFO") == 0);
}

int calculate_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    if (!validate_log_level(level)) return 0;
    
    if (strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->severity = calculate_severity(level);
    
    analyzer->entry_count++;
    return 1;
}

void calculate_statistics(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->max_severity = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) stats->error_count++;
        else if (strcmp(entry->level, "WARNING") == 0) stats->warning_count++;
        else if (strcmp(entry->level, "INFO") == 0) stats->info_count++;
        
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
    }
    
    if (stats->error_count >= stats->warning_count && stats->error_count >= stats->info_count) {
        strcpy(stats->most_frequent_level, "ERROR");
    } else if (stats->warning_count >= stats->error_count && stats->warning_count >= stats->info_count) {
        strcpy(stats->most_frequent_level, "WARNING");
    } else {
        strcpy(stats->most_frequent_level, "INFO");
    }
}

void display_statistics(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("WARNING entries: %d\n", stats->warning_count);
    printf("INFO entries: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

void display_entries_by_level(struct LogAnalyzer *analyzer, const char *level) {
    printf("Entries with level %s:\n", level);
    int count = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        if (strcmp(entry->level, level