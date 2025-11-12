//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char earliest[20];
    char latest[20];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, sizeof(analyzer->stats.earliest));
    memset(analyzer->stats.latest, 0, sizeof(analyzer->stats.latest));
}

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
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

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    char level[10];
    char timestamp[20];
    char message[MAX_LINE_LENGTH - 30];
    
    if (sscanf(line, "%19s %9s %[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    
    if (analyzer->entry_count == 0) return;
    
    strncpy(stats->earliest, analyzer->entries[0].timestamp, sizeof(stats->earliest) - 1);
    stats->earliest[sizeof(stats->earliest) - 1] = '\0';
    strncpy(stats->latest, analyzer->entries[0].timestamp, sizeof(stats->latest) - 1);
    stats->latest[sizeof(stats->latest) - 1] = '\0';
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            stats->error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            stats->warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            stats->info_count++;
        }
        
        if (compare_timestamps(entry->timestamp, stats->earliest) < 0) {
            strncpy(stats->earliest, entry->timestamp, sizeof(stats->earliest) - 1);
            stats->earliest[sizeof(stats->earliest) - 1] = '\0';
        }
        if (compare_timestamps(entry->timestamp, stats->latest) > 0) {
            strncpy(stats->latest, entry->timestamp, sizeof(stats->latest) - 1);
            stats->latest[sizeof(stats->latest) - 1] = '\0';
        }
    }
}

void print_stats(const struct LogStats *stats) {
    printf("