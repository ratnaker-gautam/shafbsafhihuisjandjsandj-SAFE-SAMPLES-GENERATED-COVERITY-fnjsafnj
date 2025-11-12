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

int parse_timestamp(const char* str, time_t* result) {
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *result = mktime(&tm);
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "ERROR") == 0 || 
           strcmp(level, "WARNING") == 0 || 
           strcmp(level, "INFO") == 0;
}

void init_analyzer(struct LogAnalyzer* analyzer) {
    memset(analyzer, 0, sizeof(*analyzer));
    analyzer->stats.earliest_time = INT32_MAX;
    analyzer->stats.latest_time = 0;
}

int add_log_entry(struct LogAnalyzer* analyzer, const char* line, uint32_t line_num) {
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) {
        return 0;
    }
    
    if (!is_valid_level(level)) {
        return 0;
    }
    
    time_t entry_time;
    if (!parse_timestamp(timestamp, &entry_time)) {
        return 0;
    }
    
    struct LogEntry* entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->line_number = line_num;
    
    analyzer->stats.total_entries++;
    if (strcmp(level, "ERROR") == 0) analyzer->stats.error_count++;
    else if (strcmp(level, "WARNING") == 0) analyzer->stats.warning_count++;
    else if (strcmp(level, "INFO") == 0) analyzer->stats.info_count++;
    
    if (entry_time < analyzer->stats.earliest_time) analyzer->stats.earliest_time = entry_time;
    if (entry_time > analyzer->stats.latest_time) analyzer->stats.latest_time = entry_time;
    
    analyzer->entry_count++;
    return 1;
}

void print_stats(const struct LogAnalyzer* analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("Errors: %u\n", analyzer->stats.error_count);
    printf("Warnings: %u\n", analyzer->stats.warning_count);
    printf("Info: %u\n", analyzer->stats.info_count);
    
    if (analyzer->stats.total_entries > 0) {
        char buffer[32];
        struct tm* tm_info = localtime(&analyzer->stats.earliest_time);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Earliest: %s\n", buffer);
        
        tm_info = localtime(&analyzer->stats.latest_time);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Latest: %s\n", buffer);
    }
}

void print_errors(const struct LogAnalyzer* analyzer) {
    printf("\nError entries:\n");
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, "ERROR") == 0) {
            printf("Line %u: %s %s %s\n", 
                   analyzer->entries[i].line_number,
                   analyzer->entries[i].timestamp,
                   analyzer->entries[i].level,
                   analyzer->entries[i].message);
        }
    }
}

int read_log_file(struct LogAnalyzer* analyzer, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len -