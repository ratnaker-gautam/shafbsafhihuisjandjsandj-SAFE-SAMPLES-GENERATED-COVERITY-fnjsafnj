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
    int error_count;
    int warning_count;
    int info_count;
    int total_entries;
    time_t earliest_time;
    time_t latest_time;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(*analyzer));
    analyzer->stats.earliest_time = 0;
    analyzer->stats.latest_time = 0;
}

int parse_timestamp(const char *timestamp_str, time_t *result) {
    struct tm tm = {0};
    if (strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *result = mktime(&tm);
    return 1;
}

int parse_log_level(const char *level_str, int *severity) {
    if (strcmp(level_str, "ERROR") == 0) {
        *severity = 3;
        return 1;
    } else if (strcmp(level_str, "WARNING") == 0) {
        *severity = 2;
        return 1;
    } else if (strcmp(level_str, "INFO") == 0) {
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    char timestamp[32], level[16], message[256];
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (!parse_log_level(level, &entry->severity)) {
        return 0;
    }
    
    time_t entry_time;
    if (!parse_timestamp(timestamp, &entry_time)) {
        return 0;
    }
    
    if (analyzer->stats.earliest_time == 0 || entry_time < analyzer->stats.earliest_time) {
        analyzer->stats.earliest_time = entry_time;
    }
    if (entry_time > analyzer->stats.latest_time) {
        analyzer->stats.latest_time = entry_time;
    }
    
    switch (entry->severity) {
        case 3: analyzer->stats.error_count++; break;
        case 2: analyzer->stats.warning_count++; break;
        case 1: analyzer->stats.info_count++; break;
    }
    
    analyzer->stats.total_entries++;
    analyzer->entry_count++;
    
    return 1;
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Errors: %d\n", analyzer->stats.error_count);
    printf("Warnings: %d\n", analyzer->stats.warning_count);
    printf("Info: %d\n", analyzer->stats.info_count);
    
    if (analyzer->stats.earliest_time > 0) {
        char time_buf[32];
        struct tm *tm_info = localtime(&analyzer->stats.earliest_time);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Earliest: %s\n", time_buf);
        
        tm_info = localtime(&analyzer->stats.latest_time);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Latest: %s\n", time_buf);
    }
}

void print_errors(const struct LogAnalyzer *analyzer) {
    printf("\nError entries:\n");
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (analyzer->entries[i].severity == 3) {
            printf("%s %s %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
        }
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    printf("Enter log entries (format: YYY