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
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    if (strlen(message) >= 256) return 0;

    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
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

    analyzer->entry_count++;
    analyzer->stats.total_entries++;
    return 1;
}

void calculate_statistics(struct LogAnalyzer *analyzer) {
    int error_count = 0, warning_count = 0, info_count = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(analyzer->entries[i].level, "WARNING") == 0) warning_count++;
        else info_count++;
    }

    analyzer->stats.error_count = error_count;
    analyzer->stats.warning_count = warning_count;
    analyzer->stats.info_count = info_count;

    if (error_count >= warning_count && error_count >= info_count) {
        strncpy(analyzer->stats.most_frequent_level, "ERROR", MAX_LEVEL_LENGTH - 1);
        analyzer->stats.max_level_count = error_count;
    } else if (warning_count >= error_count && warning_count >= info_count) {
        strncpy(analyzer->stats.most_frequent_level, "WARNING", MAX_LEVEL_LENGTH - 1);
        analyzer->stats.max_level_count = warning_count;
    } else {
        strncpy(analyzer->stats.most_frequent_level, "INFO", MAX_LEVEL_LENGTH - 1);
        analyzer->stats.max_level_count = info_count;
    }
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("ERROR entries: %d\n", analyzer->stats.error_count);
    printf("WARNING entries: %d\n", analyzer->stats.warning_count);
    printf("INFO entries: %d\n", analyzer->stats.info_count);
    printf("Most frequent level: %s (%d occurrences)\