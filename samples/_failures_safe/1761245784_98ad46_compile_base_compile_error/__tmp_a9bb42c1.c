//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
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
    return strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    struct tm tm1, tm2;
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    if (time1 == (time_t)-1 || time2 == (time_t)-1) {
        return 0;
    }
    double diff = difftime(time1, time2);
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

int parse_log_level(const char *level_str, int *severity) {
    if (strcasecmp(level_str, "ERROR") == 0) {
        *severity = 4;
        return 1;
    } else if (strcasecmp(level_str, "WARNING") == 0) {
        *severity = 3;
        return 1;
    } else if (strcasecmp(level_str, "INFO") == 0) {
        *severity = 2;
        return 1;
    } else if (strcasecmp(level_str, "DEBUG") == 0) {
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    if (strlen(timestamp) >= sizeof(analyzer->entries[0].timestamp) - 1) {
        return 0;
    }
    if (strlen(level) >= sizeof(analyzer->entries[0].level) - 1) {
        return 0;
    }
    if (strlen(message) >= sizeof(analyzer->entries[0].message) - 1) {
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
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    
    if (analyzer->entry_count == 0) {
        return;
    }
    
    strncpy(stats->earliest_timestamp, analyzer->entries[0].timestamp, sizeof(stats->earliest_timestamp) - 1);
    strncpy(stats->latest_timestamp, analyzer->entries[0].timestamp, sizeof(stats->latest_timestamp) - 1);
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        switch (entry->severity) {
            case 4: stats->error_count++; break;
            case 3: stats->warning_count++; break;
            case