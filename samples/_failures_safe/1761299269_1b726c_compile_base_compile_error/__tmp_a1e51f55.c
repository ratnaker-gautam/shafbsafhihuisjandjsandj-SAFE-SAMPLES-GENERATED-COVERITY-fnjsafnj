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

int parse_timestamp(const char *timestamp, struct tm *tm) {
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    struct tm tm1 = {0}, tm2 = {0};
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    if (time1 == -1 || time2 == -1) return 0;
    return (time1 < time2) ? -1 : (time1 > time2) ? 1 : 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level[16];
    char timestamp[32];
    char message[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    analyzer->entries[analyzer->entry_count] = *entry;
    analyzer->entries[analyzer->entry_count].line_number = analyzer->entry_count + 1;
    analyzer->entry_count++;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    } else if (strcmp(entry->level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
    
    if (analyzer->stats.earliest_timestamp[0] == '\0' || 
        compare_timestamps(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
        strncpy(analyzer->stats.earliest_timestamp, entry->timestamp, 
                sizeof(analyzer->stats.earliest_timestamp) - 1);
        analyzer->stats.earliest_timestamp[sizeof(analyzer->stats.earliest_timestamp) - 1] = '\0';
    }
    
    if (analyzer->stats.latest_timestamp[0] == '\0' || 
        compare_timestamps(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
        strncpy(analyzer->stats.latest_timestamp, entry->timestamp, 
                sizeof(analyzer->stats.latest_timestamp) - 1);
        analyzer->stats.latest_timestamp[sizeof(analyzer->stats.latest_timestamp) - 1] = '\0';
    }
}

int process_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    if (!analyzer || !filename) return 0;
    
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE