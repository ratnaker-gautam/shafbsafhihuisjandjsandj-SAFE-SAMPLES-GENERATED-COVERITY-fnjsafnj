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
    if (!timestamp || strlen(timestamp) != 19) return 0;
    if (timestamp[4] != '-' || timestamp[7] != '-' || timestamp[10] != ' ' ||
        timestamp[13] != ':' || timestamp[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp[i])) return 0;
    }
    
    int year, month, day, hour, minute, second;
    if (sscanf(timestamp, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6) return 0;
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    if (second < 0 || second > 59) return 0;
    
    return 1;
}

int validate_level(const char* level) {
    if (!level) return 0;
    return (strcmp(level, "ERROR") == 0 || strcmp(level, "WARNING") == 0 || strcmp(level, "INFO") == 0);
}

void init_analyzer(struct LogAnalyzer* analyzer) {
    if (!analyzer) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest_timestamp, 0, sizeof(analyzer->stats.earliest_timestamp));
    memset(analyzer->stats.latest_timestamp, 0, sizeof(analyzer->stats.latest_timestamp));
}

int parse_log_line(const char* line, struct LogEntry* entry, uint32_t line_num) {
    if (!line || !entry || strlen(line) == 0) return 0;
    
    char timestamp[32], level[16], message[256];
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) return 0;
    
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->line_number = line_num;
    
    return 1;
}

int compare_timestamps(const char* ts1, const char* ts2) {
    if (!ts1 || !ts2) return 0;
    return strcmp(ts1, ts2);
}

void update_stats(struct LogAnalyzer* analyzer, const struct LogEntry* entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) analyzer->stats.error_count++;
    else if (strcmp(entry->level, "WARNING") == 0) analyzer->stats.warning_count++;
    else if (strcmp(entry->level, "INFO") == 0) analyzer->stats.info_count++;
    
    if (analyzer->stats.total_entries == 1) {
        strncpy(analyzer->stats.earliest_timestamp, entry->timestamp, sizeof(analyzer->stats.earliest_timestamp) - 1);
        analyzer->stats.earliest_timestamp[sizeof(analyzer->stats.earliest_timestamp) - 1] = '\0';
        strncpy(analyzer->stats.latest_timestamp, entry->timestamp, sizeof(analyzer->stats.latest_timestamp) - 1);
        analyzer->stats.latest_timestamp[sizeof(