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
    int entry_count;
    struct LogStats stats;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    strncpy(analyzer->stats.most_frequent_level, "UNKNOWN", MAX_LEVEL_LENGTH - 1);
    analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
    analyzer->stats.max_level_count = 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[MAX_LEVEL_LENGTH];
    char message_buf[256];
    char timestamp_buf[32];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp_buf, level_buf, message_buf);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp_buf) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level_buf) >= sizeof(entry->level)) return 0;
    if (strlen(message_buf) >= sizeof(entry->message)) return 0;
    
    strncpy(entry->timestamp, timestamp_buf, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level_buf, "ERROR") == 0) entry->severity = 4;
    else if (strcmp(level_buf, "WARNING") == 0) entry->severity = 3;
    else if (strcmp(level_buf, "INFO") == 0) entry->severity = 2;
    else if (strcmp(level_buf, "DEBUG") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    if (!parse_log_line(line, entry)) return 0;
    
    analyzer->entry_count++;
    return 1;
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    
    int level_counts[5] = {0};
    char *level_names[] = {"UNKNOWN", "DEBUG", "INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        switch (entry->severity) {
            case 4: stats->error_count++; level_counts[4]++; break;
            case 3: stats->warning_count++; level_counts[3]++; break;
            case 2: stats->info_count++; level_counts[2]++; break;
            case 1: stats->debug_count++; level_counts[1]++; break;
            default: level_counts[0]++; break;
        }
    }
    
    stats->max_level_count = 0;
    for (int i = 0; i < 5; i++) {
        if (level_counts[i] > stats->max_level_count) {
            stats->max_level_count = level_counts[i];
            strncpy(stats->most_frequent_level, level_names[i], MAX_LEVEL_LENGTH - 1);
            stats->most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
        }
    }
}

void print_stats(const struct LogStats *stats) {
    if (stats == NULL) return;