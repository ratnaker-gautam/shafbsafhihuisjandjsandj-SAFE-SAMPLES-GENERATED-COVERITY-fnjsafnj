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
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int parse_log_level(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 0;
    return -1;
}

int compare_timestamps(const char* ts1, const char* ts2) {
    return strcmp(ts1, ts2);
}

void init_analyzer(struct LogAnalyzer* analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest_timestamp, 0, 32);
    memset(analyzer->stats.latest_timestamp, 0, 32);
}

int add_log_entry(struct LogAnalyzer* analyzer, const char* timestamp, 
                  const char* level, const char* message, uint32_t line_num) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (parse_log_level(level) < 0) return 0;
    if (strlen(message) >= 256) return 0;
    
    struct LogEntry* entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->line_number = line_num;
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer* analyzer) {
    if (analyzer->entry_count == 0) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    
    strcpy(analyzer->stats.earliest_timestamp, analyzer->entries[0].timestamp);
    strcpy(analyzer->stats.latest_timestamp, analyzer->entries[0].timestamp);
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry* entry = &analyzer->entries[i];
        int level = parse_log_level(entry->level);
        
        if (level == 2) analyzer->stats.error_count++;
        else if (level == 1) analyzer->stats.warning_count++;
        else if (level == 0) analyzer->stats.info_count++;
        
        if (compare_timestamps(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
            strcpy(analyzer->stats.earliest_timestamp, entry->timestamp);
        }
        if (compare_timestamps(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
            strcpy(analyzer->stats.latest_timestamp, entry->timestamp);
        }
    }
}

void print_stats(const struct LogAnalyzer* analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("Errors: %u\n", analyzer->stats.error_count);
    printf("Warnings: %u\n", analyzer->stats.warning_count);
    printf("Info: %u\n", analyzer->stats.info_count);
    printf("Time range: %s to %s\n", 
           analyzer->stats.earliest_timestamp, 
           analyzer->stats.latest_timestamp);
}

void print_errors(const struct LogAnalyzer* analyzer) {
    printf("\nError entries:\n");
    for (uint32_t i = 0; i < analyzer->