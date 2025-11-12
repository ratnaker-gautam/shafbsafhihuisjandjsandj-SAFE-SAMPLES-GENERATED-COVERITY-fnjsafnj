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

void init_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(*analyzer));
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->entry_count = 0;
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
    return difftime(time1, time2);
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char level_buf[MAX_LEVEL_LENGTH];
    char timestamp_buf[32];
    char message_buf[MAX_MESSAGE_LENGTH];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    if (parsed != 3) return 0;
    
    int severity = validate_level(level_buf);
    if (severity == 0) return 0;
    
    strncpy(entry->timestamp, timestamp_buf, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->severity = severity;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_entries++;
    
    switch (entry->severity) {
        case 4: analyzer->stats.error_count++; break;
        case 3: analyzer->stats.warning_count++; break;
        case 2: analyzer->stats.info_count++; break;
        case 1: analyzer->stats.debug_count++; break;
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
    printf("Errors: