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
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_level_count = 0;
    memset(analyzer->stats.most_frequent_level, 0, MAX_LEVEL_LENGTH);
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char *level) {
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    const char *valid_levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    if (!validate_timestamp(timestamp) || !validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 4;
    else if (strcmp(level, "WARNING") == 0) entry->severity = 3;
    else if (strcmp(level, "INFO") == 0) entry->severity = 2;
    else entry->severity = 1;
    
    return 1;
}

void add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return;
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        analyzer->entries[analyzer->entry_count] = entry;
        analyzer->entry_count++;
    }
}

void calculate_stats(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    
    int level_counts[4] = {0};
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
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
    
    stats->max_level_count = 0