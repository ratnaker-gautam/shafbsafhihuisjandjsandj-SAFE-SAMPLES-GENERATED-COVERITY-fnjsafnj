//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    uint32_t debug_count;
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.earliest_timestamp[0] = '\0';
    analyzer->stats.latest_timestamp[0] = '\0';
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    if (timestamp[4] != '-' || timestamp[7] != '-' || timestamp[10] != ' ' ||
        timestamp[13] != ':' || timestamp[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp[i])) return 0;
    }
    return 1;
}

int parse_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 3;
    if (strcmp(level, "DEBUG") == 0) return 4;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line, uint32_t line_num) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    char timestamp[32], level[16], message[256];
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (!parse_log_level(level)) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->timestamp[31] = '\0';
    entry->level[15] = '\0';
    entry->message[255] = '\0';
    entry->line_number = line_num;
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    memset(&analyzer->stats, 0, sizeof(struct LogStats));
    analyzer->stats.total_entries = analyzer->entry_count;
    
    if (analyzer->entry_count == 0) return;
    
    strncpy(analyzer->stats.earliest_timestamp, 
            analyzer->entries[0].timestamp, 31);
    strncpy(analyzer->stats.latest_timestamp, 
            analyzer->entries[analyzer->entry_count - 1].timestamp, 31);
    analyzer->stats.earliest_timestamp[31] = '\0';
    analyzer->stats.latest_timestamp[31] = '\0';
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        const char *level = analyzer->entries[i].level;
        if (strcmp(level, "ERROR") == 0) analyzer->stats.error_count++;
        else if (strcmp(level, "WARNING") == 0) analyzer->stats.warning_count++;
        else if (strcmp(level, "INFO") == 0) analyzer->stats.info_count++;
        else if (strcmp(level, "DEBUG") == 0) analyzer->stats.debug_count++;
    }
}

void print_analysis(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("Error count: %u\n", analyzer->stats.error_count);
    printf("Warning count: %u\n", analyzer->stats.warning_count);
    printf("Info count: %u\n", analyzer->stats.info_count);
    printf("Debug count: %u\n", analyzer->stats.debug_count);
    printf("Time range: %s to %s\n", 
           analyzer->stats.earliest_timestamp,
           analyzer->stats.latest_timestamp);
    
    if (analyzer->stats.error_count > 0) {
        printf("\nError entries:\n");
        for (uint32_t i = 0; i < analyzer->entry_count; i++) {
            if (strcmp(analyzer->entries[i].