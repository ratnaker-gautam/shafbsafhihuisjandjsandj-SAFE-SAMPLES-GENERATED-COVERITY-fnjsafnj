//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    time_t earliest_time;
    time_t latest_time;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
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

int validate_log_level(const char *level) {
    return (strcmp(level, "ERROR") == 0 || 
            strcmp(level, "WARNING") == 0 || 
            strcmp(level, "INFO") == 0);
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    char timestamp[32], level[16], message[256];
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) {
        return 0;
    }
    
    if (!validate_log_level(level)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, 31);
    strncpy(entry->level, level, 15);
    strncpy(entry->message, message, 255);
    entry->timestamp[31] = '\0';
    entry->level[15] = '\0';
    entry->message[255] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    time_t entry_time;
    if (parse_timestamp(entry->timestamp, &entry_time)) {
        if (analyzer->stats.earliest_time == 0 || entry_time < analyzer->stats.earliest_time) {
            analyzer->stats.earliest_time = entry_time;
        }
        if (analyzer->stats.latest_time == 0 || entry_time > analyzer->stats.latest_time) {
            analyzer->stats.latest_time = entry_time;
        }
    }
}

int process_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        entry.line_number = line_num;
        
        if (parse_log_line(line, &entry)) {
            analyzer->entries[analyzer->entry_count] = entry;
            update_stats(analyzer, &entry);
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void print_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("Errors: %u\n", analyzer->stats.error_count);
    printf("Warnings: %u\n", analyzer->stats.warning_count);
    printf("Info: %u\n", analyzer->stats.info_count);
    
    if (analyzer->stats.earliest_time > 0 && analyzer->stats.latest_time > 0) {
        char earliest[32], latest[32];
        struct tm tm_early, tm_late;
        struct tm *tm_early_ptr = localtime(&analyzer->stats.earliest_time);
        struct tm *tm_late_ptr = localtime(&analyzer->stats.latest_time);
        
        if (tm_early_ptr && tm_late_ptr) {
            memcpy(&tm_early, tm_early_ptr, sizeof(struct tm));
            memcpy(&tm_late, tm_late_ptr, sizeof(struct tm));
            str