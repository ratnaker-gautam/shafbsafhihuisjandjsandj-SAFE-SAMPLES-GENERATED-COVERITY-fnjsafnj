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
    
    int year, month, day, hour, minute, second;
    if (sscanf(timestamp, "%4d-%2d-%2d %2d:%2d:%2d", 
               &year, &month, &day, &hour, &minute, &second) != 6) return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    if (second < 0 || second > 59) return 0;
    
    return 1;
}

int parse_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 3;
    if (strcmp(level, "DEBUG") == 0) return 4;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message, uint32_t line_num) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (!parse_log_level(level)) return 0;
    if (strlen(message) == 0 || strlen(message) >= 255) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
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

void update_stats(struct LogAnalyzer *analyzer) {
    memset(&analyzer->stats, 0, sizeof(struct LogStats));
    analyzer->stats.total_entries = analyzer->entry_count;
    
    if (analyzer->entry_count == 0) return;
    
    strcpy(analyzer->stats.earliest_timestamp, analyzer->entries[0].timestamp);
    strcpy(analyzer->stats.latest_timestamp, analyzer->entries[0].timestamp);
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        const char *level = analyzer->entries[i].level;
        if (strcmp(level, "ERROR") == 0) analyzer->stats.error_count++;
        else if (strcmp(level, "WARNING") == 0) analyzer->stats.warning_count++;
        else if (strcmp(level, "INFO") == 0) analyzer->stats.info_count++;
        else if (strcmp(level, "DEBUG") == 0) analyzer->stats.debug_count++;
        
        if (strcmp(analyzer->entries[i].timestamp, analyzer->stats.earliest_timestamp) < 0) {
            strcpy(analyzer->stats.earliest_timestamp, analyzer->entries[i].timestamp);
        }
        if (strcmp(analyzer->entries[i].timestamp, analyzer->stats.latest_timestamp) > 0) {
            strcpy(analyzer->stats.latest_timestamp, analyzer->entries[i].timestamp);
        }
    }
}

void print