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
    char most_frequent_level[MAX_LEVEL_LENGTH];
    int max_severity;
    time_t earliest_time;
    time_t latest_time;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
    analyzer->stats.max_severity = 0;
    analyzer->stats.earliest_time = 0;
    analyzer->stats.latest_time = 0;
}

int parse_timestamp(const char *timestamp_str, time_t *result) {
    struct tm tm = {0};
    if (strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *result = mktime(&tm);
    return 1;
}

int calculate_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= MAX_LEVEL_LENGTH || strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = calculate_severity(level);
    
    analyzer->entry_count++;
    return 1;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    if (analyzer->entry_count == 0) return;
    
    int level_counts[4] = {0};
    char *level_names[] = {"INFO", "WARNING", "ERROR", "UNKNOWN"};
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_severity = 0;
    
    time_t first_time = 0, last_time = 0;
    int first_set = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) {
            analyzer->stats.error_count++;
            level_counts[2]++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            analyzer->stats.warning_count++;
            level_counts[1]++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            analyzer->stats.info_count++;
            level_counts[0]++;
        } else {
            level_counts[3]++;
        }
        
        if (entry->severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = entry->severity;
        }
        
        time_t entry_time;
        if (parse_timestamp(entry->timestamp, &entry_time)) {
            if (!first_set) {
                first_time = entry_time;
                last_time = entry_time;
                first_set = 1;
            } else {
                if (entry_time < first_time) first_time = entry_time;
                if (entry_time > last_time) last_time = entry_time;
            }
        }
    }
    
    int max_count = 0;
    int max_index = 0;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    strcpy(analyzer->stats.most_frequent_level, level_names[max_index]);
    
    if (first_set) {
        analyzer->stats.earliest_time = first_time;
        analyzer->stats.latest_time = last_time;
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("INFO entries: %d\n", analyzer->stats.info_count);
    printf("WARNING entries: %d\n", analyzer->stats.warning_count);