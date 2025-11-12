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
    char most_frequent_level[MAX_LEVEL_LENGTH];
    int max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_level_count = 0;
    memset(analyzer->stats.most_frequent_level, 0, MAX_LEVEL_LENGTH);
}

int parse_timestamp(const char *timestamp_str, char *output) {
    if (timestamp_str == NULL || output == NULL) return 0;
    
    struct tm tm_time;
    memset(&tm_time, 0, sizeof(struct tm));
    
    if (strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
        strftime(output, 32, "%Y-%m-%d %H:%M:%S", &tm_time);
        return 1;
    }
    
    strncpy(output, timestamp_str, 31);
    output[31] = '\0';
    return 1;
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    
    const char *valid_levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int calculate_severity(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (!validate_level(level)) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->severity = calculate_severity(level);
    
    analyzer->entry_count++;
    return 1;
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    struct LogStats *stats = &analyzer->stats;
    int level_counts[4] = {0};
    const char *level_names[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        const char *level = analyzer->entries[i].level;
        
        if (strcmp(level, "ERROR") == 0) {
            stats->error_count++;
            level_counts[0]++;
        } else if (strcmp(level, "WARNING") == 0) {
            stats->warning_count++;
            level_counts[1]++;
        } else if (strcmp(level, "INFO") == 0) {
            stats->info_count++;
            level_counts[2]++;
        } else if (strcmp(level, "DEBUG") == 0) {
            stats->debug_count++;
            level_counts[3]++;
        }
    }
    
    stats->max_level_count = 0;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > stats->max_level_count) {
            stats->max_level_count = level_counts[i];
            strncpy(stats->most_frequent_level, level_names[i], MAX_LEVEL_LENGTH - 1);
            stats->most_frequent_level[MAX_LEVEL_LENGTH -