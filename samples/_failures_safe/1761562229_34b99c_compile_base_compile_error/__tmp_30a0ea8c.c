//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t line_number;
};

struct LogStats {
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    uint32_t total_entries;
    char most_frequent_level[16];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.total_entries = 0;
    strncpy(analyzer->stats.most_frequent_level, "UNKNOWN", sizeof(analyzer->stats.most_frequent_level) - 1);
    analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    size_t line_len = strlen(line);
    if (line_len >= MAX_LINE_LENGTH - 1) return 0;
    
    strncpy(temp_line, line, sizeof(temp_line) - 1);
    temp_line[sizeof(temp_line) - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
}

void calculate_most_frequent_level(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    uint32_t max_count = analyzer->stats.error_count;
    strncpy(analyzer->stats.most_frequent_level, "ERROR", sizeof(analyzer->stats.most_frequent_level) - 1);
    analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
    
    if (analyzer->stats.warning_count > max_count) {
        max_count = analyzer->stats.warning_count;
        strncpy(analyzer->stats.most_frequent_level, "WARNING", sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
    }
    
    if (analyzer->stats.info_count > max_count) {
        strncpy(analyzer->stats.most_frequent_level, "INFO", sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("ERROR entries: %u\n", analyzer->stats.error_count);
    printf("WARNING entries: %u\n", analyzer->stats.warning_count);
    printf("INFO entries: %u\n", analyzer->stats.info_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
}

int read_log_file(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL