//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 10

struct LogEntry {
    char timestamp[20];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char earliest_timestamp[20];
    char latest_timestamp[20];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest_timestamp, 0, 20);
    memset(analyzer->stats.latest_timestamp, 0, 20);
}

int parse_timestamp(const char *timestamp, struct tm *tm) {
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    struct tm tm1, tm2;
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    if (time1 == -1 || time2 == -1) {
        return 0;
    }
    return difftime(time1, time2);
}

int validate_log_entry(const char *timestamp, const char *level, const char *message) {
    if (!timestamp || !level || !message) {
        return 0;
    }
    if (strlen(timestamp) >= 20 || strlen(level) >= MAX_LEVEL_LENGTH || strlen(message) >= 256) {
        return 0;
    }
    struct tm tm;
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
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
        analyzer->stats.error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
        analyzer->stats.warning_count++;
    } else {
        entry->severity = 1;
        analyzer->stats.info_count++;
    }
    
    if (analyzer->stats.total_entries == 0) {
        strncpy(analyzer->stats.earliest_timestamp, timestamp, 19);
        strncpy(analyzer->stats.latest_timestamp, timestamp, 19);
    } else {
        if (compare_timestamps(timestamp, analyzer->stats.earliest_timestamp) < 0) {
            strncpy(analyzer->stats.earliest_timestamp, timestamp, 19);
        }
        if (compare_timestamps(timestamp, analyzer->stats.latest_timestamp) > 0) {
            strncpy(analyzer->stats.latest_timestamp, timestamp, 19);
        }
    }
    
    analyzer->entry_count++;
    analyzer->stats.total_entries++;
    return 1;
}

void generate_statistics(struct LogAnalyzer *analyzer) {
    if (!analyzer) {
        return;
    }
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Error count: %d\n", analyzer->stats.error_count);
    printf("Warning count: %d\n", analyzer->stats.warning_count);
    printf("Info count: %d\n", analyzer->stats.info_count);
    printf("Time range: %s to %s\n", analyzer->stats.earliest_timestamp, analyzer->stats.latest_timestamp);
    printf("\n");
}

void display_entries_by_level(struct LogAnalyzer *analyzer, const char