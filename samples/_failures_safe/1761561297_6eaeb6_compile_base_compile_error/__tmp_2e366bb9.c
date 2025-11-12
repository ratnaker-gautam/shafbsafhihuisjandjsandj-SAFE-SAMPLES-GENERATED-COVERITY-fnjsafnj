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
    if (time1 == -1 || time2 == -1) return 0;
    return (time1 < time2) ? -1 : (time1 > time2) ? 1 : 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || strlen(line) < 30) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) != 19 || timestamp[10] != ' ') return 0;
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    
    strncpy(entry->message, message, MAX_LINE_LENGTH - 30);
    entry->message[MAX_LINE_LENGTH - 30 - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    if (analyzer->stats.total_entries == 1) {
        strcpy(analyzer->stats.earliest_timestamp, entry->timestamp);
        strcpy(analyzer->stats.latest_timestamp, entry->timestamp);
    } else {
        if (compare_timestamps(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
            strcpy(analyzer->stats.earliest_timestamp, entry->timestamp);
        }
        if (compare_timestamps(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
            strcpy(analyzer->stats.latest_timestamp, entry->timestamp);
        }
    }
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) return 0;
    
    analyzer->entries[analyzer->entry_count] = entry;
    update_stats(analyzer, &entry);
    analyzer->entry_count++;
    
    return 1;
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Errors: %d\n", analyzer->stats.error_count);
    printf("Warnings: %d\n", analyzer->stats.warning_count);
    printf("Info: %d\n", analyzer->stats.info_count);
    printf("Time range: %s to %s\n", 
           analyzer->stats.earliest_timestamp,
           analyzer->stats.latest_timestamp);
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("\n