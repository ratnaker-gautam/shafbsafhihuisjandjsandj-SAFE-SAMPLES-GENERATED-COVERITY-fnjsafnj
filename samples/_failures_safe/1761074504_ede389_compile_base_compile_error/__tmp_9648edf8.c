//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    char most_frequent_level[16];
    int max_severity;
    time_t earliest_time;
    time_t latest_time;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

int parse_timestamp(const char* timestamp_str, time_t* result) {
    struct tm tm = {0};
    if (strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *result = mktime(&tm);
    return 1;
}

int validate_log_level(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARN") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32], level[16], message[256];
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    
    time_t time_val;
    if (!parse_timestamp(timestamp, &time_val)) return 0;
    
    int severity = validate_log_level(level);
    if (severity == 0) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->severity = severity;
    return 1;
}

void initialize_analyzer(struct LogAnalyzer* analyzer) {
    if (analyzer == NULL) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.earliest_time = -1;
    analyzer->stats.latest_time = -1;
}

void calculate_stats(struct LogAnalyzer* analyzer) {
    if (analyzer == NULL || analyzer->entry_count == 0) return;
    
    struct LogStats* stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    
    int level_counts[5] = {0};
    stats->max_severity = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry* entry = &analyzer->entries[i];
        
        time_t entry_time;
        if (parse_timestamp(entry->timestamp, &entry_time)) {
            if (stats->earliest_time == -1 || entry_time < stats->earliest_time) {
                stats->earliest_time = entry_time;
            }
            if (stats->latest_time == -1 || entry_time > stats->latest_time) {
                stats->latest_time = entry_time;
            }
        }
        
        if (entry->severity >= 0 && entry->severity < 5) {
            level_counts[entry->severity]++;
        }
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
    }
    
    stats->error_count = level_counts[4];
    stats->warning_count = level_counts[3];
    stats->info_count = level_counts[2];
    stats->debug_count = level_counts[1];
    
    const char* most_freq = "DEBUG";
    int max_count = level_counts[1];
    if (level_counts[2] > max_count) { most_freq = "INFO"; max_count = level_counts[2]; }
    if (level_counts[3] > max_count) { most_freq = "WARN"; max_count = level_counts[3]; }
    if (level_counts[4] > max_count) { most_freq = "ERROR"; max_count = level_counts[4]; }
    
    strncpy(stats->most_frequent_level, most_freq, sizeof(stats->most_frequent_level) - 1);
    stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = '\0';
}

int add_log_entry(struct LogAnalyzer* analyzer, const char* line) {
    if (analyzer == NULL || line == NULL || analyzer->entry_count >= MAX_ENTRIES) {
        return 0;
    }
    
    struct LogEntry* entry = &analyzer