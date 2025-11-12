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

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    memset(analyzer->stats.earliest_timestamp, 0, 32);
    memset(analyzer->stats.latest_timestamp, 0, 32);
    analyzer->entry_count = 0;
}

int parse_timestamp(const char *str, struct tm *tm) {
    if (!str || !tm) return 0;
    return sscanf(str, "%d-%d-%d %d:%d:%d", 
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    if (!ts1 || !ts2) return 0;
    struct tm tm1 = {0}, tm2 = {0};
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    if (time1 == -1 || time2 == -1) return 0;
    return (time1 < time2) ? -1 : (time1 > time2) ? 1 : 0;
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (!line || !entry) return 0;
    
    char level_buf[16];
    char timestamp_buf[32];
    char message_buf[256];
    
    int result = sscanf(line, "%31s %15s %255[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    
    if (result < 3) return 0;
    
    if (strlen(timestamp_buf) >= 32 || strlen(level_buf) >= 16 || strlen(message_buf) >= 256) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp_buf, 31);
    strncpy(entry->level, level_buf, 15);
    strncpy(entry->message, message_buf, 255);
    entry->line_number = line_num;
    
    entry->timestamp[31] = '\0';
    entry->level[15] = '\0';
    entry->message[255] = '\0';
    
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
    
    if (analyzer->stats.total_entries == 1) {
        strncpy(analyzer->stats.earliest_timestamp, entry->timestamp, 31);
        strncpy(analyzer->stats.latest_timestamp, entry->timestamp, 31);
        analyzer->stats.earliest_timestamp[31] = '\0';
        analyzer->stats.latest_timestamp[31] = '\0';
    } else {
        if (compare_timestamps(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
            strncpy(analyzer->stats.earliest_timestamp, entry->timestamp, 31);
            analyzer->stats.earliest_timestamp[31] = '\0';
        }
        if (compare_timestamps(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
            strncpy(analyzer->stats.latest_timestamp, entry->timestamp, 31);
            analyzer->stats.latest_timestamp[31] = '\0';
        }
    }
}

int process_log_file(struct LogAnalyzer *analyzer, const char *filename