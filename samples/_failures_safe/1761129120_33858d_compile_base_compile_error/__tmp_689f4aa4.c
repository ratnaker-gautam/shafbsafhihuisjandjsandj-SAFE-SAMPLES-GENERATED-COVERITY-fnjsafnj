//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16
#define MAX_MESSAGE_LENGTH 256

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    int debug_count;
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    memset(analyzer->stats.earliest_timestamp, 0, sizeof(analyzer->stats.earliest_timestamp));
    memset(analyzer->stats.latest_timestamp, 0, sizeof(analyzer->stats.latest_timestamp));
}

int parse_timestamp(const char *timestamp_str, struct tm *tm) {
    if (timestamp_str == NULL || tm == NULL) return 0;
    return strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    struct tm tm1 = {0}, tm2 = {0};
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) return 0;
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    if (time1 == -1 || time2 == -1) return 0;
    return (time1 < time2) ? -1 : (time1 > time2) ? 1 : 0;
}

int parse_log_level(const char *level, int *severity) {
    if (level == NULL || severity == NULL) return 0;
    
    if (strcasecmp(level, "ERROR") == 0) {
        *severity = 4;
        return 1;
    } else if (strcasecmp(level, "WARNING") == 0) {
        *severity = 3;
        return 1;
    } else if (strcasecmp(level, "INFO") == 0) {
        *severity = 2;
        return 1;
    } else if (strcasecmp(level, "DEBUG") == 0) {
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (!parse_log_level(level, &entry->severity)) return 0;
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->entry_count == 0) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    
    strncpy(analyzer->stats.earliest_timestamp, analyzer->entries[0].timestamp, 
            sizeof(analyzer->stats.earliest_timestamp) - 1);
    analyzer->stats.earliest_timestamp[sizeof(analyzer->stats.earliest_timestamp) - 1] = '\0';
    strncpy(analyzer->stats.latest_timestamp, analyzer->entries[0].timestamp, 
            sizeof(analyzer->stats.latest_timestamp) - 1);
    analyzer->stats.latest_timestamp[sizeof(analyzer