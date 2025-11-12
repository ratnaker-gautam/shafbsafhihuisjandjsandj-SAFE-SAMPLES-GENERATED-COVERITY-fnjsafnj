//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    int debug_count;
    char earliest[32];
    char latest[32];
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
    memset(analyzer->stats.earliest, 0, sizeof(analyzer->stats.earliest));
    memset(analyzer->stats.latest, 0, sizeof(analyzer->stats.latest));
}

int parse_timestamp(const char *timestamp, struct tm *tm) {
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    struct tm tm1 = {0}, tm2 = {0};
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    return (int)difftime(t1, t2);
}

int validate_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARN") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    if (strlen(timestamp) >= sizeof(analyzer->entries[0].timestamp) ||
        strlen(level) >= sizeof(analyzer->entries[0].level) ||
        strlen(message) >= sizeof(analyzer->entries[0].message)) {
        return 0;
    }
    
    int severity = validate_log_level(level);
    if (severity == 0) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
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
    
    strncpy(stats->earliest, analyzer->entries[0].timestamp, sizeof(stats->earliest) - 1);
    stats->earliest[sizeof(stats->earliest) - 1] = '\0';
    strncpy(stats->latest, analyzer->entries[0].timestamp, sizeof(stats->latest) - 1);
    stats->latest[sizeof(stats->latest) - 1] = '\0';
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (entry->severity == 4) stats->error_count++;
        else if (entry->severity == 3) stats->warning_count++;
        else if (entry->severity == 2) stats->info_count++;
        else if (entry->severity == 1) stats->debug_count++;
        
        if (compare_timestamps(entry->timestamp, stats->earliest) < 0) {
            strncpy(stats->earliest, entry->timestamp, sizeof(stats->earliest) - 1);
            stats->earliest[sizeof(stats->earliest) - 1] = '\0';
        }
        if (compare_timestamps(entry->timestamp, stats->latest) > 0) {
            strncpy(st