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
    char earliest[32];
    char latest[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, sizeof(analyzer->stats.earliest));
    memset(analyzer->stats.latest, 0, sizeof(analyzer->stats.latest));
}

int parse_timestamp(const char *timestamp, struct tm *tm) {
    if (!timestamp || !tm) return 0;
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    if (!ts1 || !ts2) return 0;
    struct tm tm1, tm2;
    memset(&tm1, 0, sizeof(tm1));
    memset(&tm2, 0, sizeof(tm2));
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    if (time1 == -1 || time2 == -1) {
        return 0;
    }
    double diff = difftime(time1, time2);
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

int validate_log_entry(const char *timestamp, const char *level, const char *message) {
    if (!timestamp || !level || !message) {
        return 0;
    }
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) {
        return 0;
    }
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    if (!parse_timestamp(timestamp, &tm)) {
        return 0;
    }
    if (strcmp(level, "ERROR") != 0 && strcmp(level, "WARNING") != 0 && strcmp(level, "INFO") != 0) {
        return 0;
    }
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (!analyzer || !timestamp || !level || !message) {
        return 0;
    }
    if (analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    if (!validate_log_entry(timestamp, level, message)) {
        return 0;
    }
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
    } else {
        entry->severity = 1;
    }
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (!analyzer || analyzer->entry_count == 0) {
        return;
    }
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    
    strncpy(analyzer->stats.earliest, analyzer->entries[0].timestamp, sizeof(analyzer->stats.earliest) - 1);
    analyzer->stats.earliest[sizeof(analyzer->stats.earliest) - 1] = '\0';
    strncpy(analyzer->stats.latest, analyzer->entries[0].timestamp, sizeof(analyzer->stats.latest) - 1);
    analyzer->stats.latest[sizeof(analyzer->stats.latest) - 1] = '\0';
    
    for (int i = 0; i < analyzer->entry_count