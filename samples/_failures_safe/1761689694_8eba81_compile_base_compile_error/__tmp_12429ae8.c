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

int validate_timestamp(const char *timestamp) {
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    const char *valid_levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char timestamp[20], level[MAX_LEVEL_LENGTH], message[256];
    int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    
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

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) analyzer->stats.error_count++;
    else if (strcmp(entry->level, "WARNING") == 0) analyzer->stats.warning_count++;
    else if (strcmp(entry->level, "INFO") == 0) analyzer->stats.info_count++;
    else analyzer->stats.debug_count++;
}

void calculate_frequent_level(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    int counts[4] = {0};
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
    counts[0] = analyzer->stats.error_count;
    counts[1] = analyzer->stats.warning_count;
    counts[2] = analyzer->stats.info_count;
    counts[3] = analyzer->stats.debug_count;
    
    int max_index = 0;
    for (int i = 1; i < 4; i++) {
        if (counts[i] > counts[max_index]) max_index = i;
    }
    
    if (counts[max_index] > 0) {
        strncpy(analyzer->stats.most_frequent_level, levels[max_index], MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
        analyzer->stats.max_level_count = counts[max_index];
    }
}

void display_stats(const struct LogAnalyzer *analyzer) {